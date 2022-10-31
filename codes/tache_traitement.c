#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
  char requete;
  char reqstr[1];
  char reponse[25];
  char rep_len[3];
  int transmission[2];
  int fd = atoi(argv[1]);
  printf("salut %d\n", fd);
  int err = pipe(transmission);
  if(err < 0){
    fprintf(stderr, "Erreur de création du pipe %d %d\n", err, errno);
    return 1;
  }
  pid_t tache_reponse  = fork();
  if(tache_reponse < 0){
    fprintf(stderr, "erreur de création du nouveau processus %d %d\n", tache_reponse, errno);
    return 1;
  }
  if(tache_reponse == 0){
    close(transmission[1]);
    char r1 [5]; 
    sprintf(r1, "%d", transmission[0]);
    char *argvrep [] = {"./tache_reponse", r1, (char *)0};
    err = execve("./tache_reponse", argvrep,NULL);
    fprintf(stderr, "Erreur d'execution du code %d %d %s %s\n", err, errno, argvrep[1], argvrep[2]);
  }else{
    while(1){
      int ret = read(fd, reqstr, 1);
      int size;
      requete = reqstr[0];
      if(requete == '\n')
        continue; 
      if(ret > 0){
        switch(requete){
          case 'a':
            strcpy(reponse, "BENIN");
            sprintf(rep_len, "%s", "05");
            size = 5;
            break;
          case 'b':
            strcpy(reponse, "TOGO");
            sprintf(rep_len, "%s", "04");
            size = 4;
            break;
          case 'c':
            strcpy(reponse, "GHANA");
            sprintf(rep_len, "%s", "05");
            size = 5;
            break;
          default:
            strcpy(reponse, "REQUETE NON SUPPORTEE");
            sprintf(rep_len, "%s", "21");
            size = 21;
            break;
        }
        write(transmission[1], rep_len, 2);
        write(transmission[1], reponse, size);
      }
    }
    int status = 0;
    pid_t pid = wait(&status);
    fprintf(stderr, "Le fils (%d) vient de se terminer avec (%d) %d\n", tache_reponse, status, transmission[1]);
  }
}
