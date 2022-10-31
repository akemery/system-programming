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
   int err = pipe(reception);
   if(err < 0){
     fprintf(stderr, "Erreur de création du pipe %d %d\n", err, errno);
     return 1;
   }
   pid_t tache_traitement  = fork();
   if(tache_traitement < 0){
      fprintf(stderr, "erreur de création du nouveau processus %d %d\n", tache_traitement, errno);
      return 1;
   }
   if(tache_traitement == 0){
       close(reception[1]);
       char r1 [5]; 
       sprintf(r1, "%d", reception[0]);
       //memcpy(r1, &reception[0], sizeof(int));
       char *argvreq [] = {"./tache_traitement", r1, (char *)0};
       err = execve("./tache_traitement", argvreq,NULL);
       fprintf(stderr, "Erreur d'execution du code %d %d %s %s\n", err, errno, argvreq[1], argvreq[2]);
   }else{
       char requete;
       char reqstr[1];
       close(reception[0]);
       while(1){
          scanf("%c", &requete);
          memcpy(reqstr, &requete, 1);
          write(reception[1], reqstr, 1);
       }
       int status = 0;
       pid_t pid = wait(&status);
       fprintf(stderr, "Le fils (%d) vient de se terminer avec (%d) %d\n", tache_traitement, status, reception[1]);
   }
   return 0;
}
