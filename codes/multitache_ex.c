#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define REPONSE_SIZE 30
int main(int argc, char *argv[]){
   int reception[2];
   int transmission[2];
   int err = pipe(reception);
   if(err < 0){
     fprintf(stderr, "Erreur de création du pipe %d %d\n", err, errno);
     return 1;
   }
   pid_t requete_tache  = fork();
   if(requete_tache < 0){
      fprintf(stderr, "erreur de création du nouveau processus %d %d\n", requete_tache, errno);
      return 1;
   }
   if(requete_tache == 0){
       close(reception[1]);
       char r1 [5]; 
       sprintf(r1, "%d", reception[0]);
       char *argvreq [] = {"./tache_requete", r1, (char *)0};
       err = execve("./tache_requete", argvreq,NULL);
       if(err < 0)
         fprintf(stderr, "Erreur d'execution du code %d %d %s %s\n", err, errno, argvreq[1], argvreq[2]);
   }else{
       while(1){
          char requete;
          char reqstr[1];
          close(reception[0]);
          scanf("%c", &requete);
          memcpy(reqstr, &requete, 1);
          write(reception[1], reqstr, 1);
       }
       int status = 0;
       pid_t pid = wait(&status);
       fprintf(stderr, "Le fils (%d) vient de se terminer avec (%d) %d\n", requete_tache, status, reception[1]);
   }
   return 0;
}
