#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc , char *argv[]){
  pid_t pid = fork();
  if(pid < 0){
    /* En cas d'erreur il faut mettre fin au programme*/
    fprintf(stderr, "Erreur de création du processus (%d) (%d)\n", pid, errno);
    return 1;
  }
  if(pid == 0){
     /* Nous sommes dans le processus fils*/
     printf("je suis le fils (%d), mon père est (%d)\n", getpid(), getppid());
  }else{
    /* Nous sommes dans le père*/
    int status;
    pid_t pid2 = wait(&status);
    printf("Je suis le père (%d) mon fils que je viens de créer (%d)" 
           "s'est terminé avec le code d'erreur (%d)", getpid(), pid, status);
  }
  printf("Ou suis-je?\n");
  return 0;
}
