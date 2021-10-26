#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
int main(int argc, char *argv[]){
  int i;
  pid_t pid[4];
  for(i = 0; i < 4; i++){
    pid[i] = fork();
    if(pid[i]<0){
      /*On gère l'erreur*/
      fprintf(stderr, "Erreur lors de la création du fils(%d)\n", errno);
      return 1;
    }
    if(pid[i] == 0){
      /* Dans le fils*/
      fprintf(stdout, "Je suis (%d), mon père (%d)\n", getpid(), getppid());
      break;
    }
    else{
   
    }
  }
  
  for(i = 0; i < 4; i++){
      int status;
      wait(&status);
  }
}
