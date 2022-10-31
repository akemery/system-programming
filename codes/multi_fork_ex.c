
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
  pid_t pid[4];
  for(int i = 0; i < 4; i++){
    pid[i] = fork();
    if(pid[i] < 0){
      fprintf(stderr, "Erreur de création du processus (%d)\n", errno);
      return 1;
    }
    if(pid[i] == 0){
      fprintf(stderr, "Je suis le processus fils (%d) qui vient d'être créé par (%d)\n", getpid(), getppid());
      return 0;
    }else{
      fprintf(stderr, "Je suis le processus (%d) qui vient de créer le processus (%d)\n", getpid(), pid[i]);
    }
  }
  int status;
  for(int i = 0; i < 4; i++){
    pid[i] = wait(&status);
    fprintf(stderr, "Mon fils (%d) vient de se terminer (%d)\n", pid[i], status);
  }
  return 0;
}
