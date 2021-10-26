#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
int main(int argc, char *argv[]){
  if(argc!=2){
    fprintf(stderr, "%s <entier ou chaine de caractere>\n", argv[0]);
    return 1;
  }
  int n = atoi(argv[1]);
  pid_t pid = fork();
  if(pid < 0){
    fprintf(stderr, "Erreur de création du processus (%d)\n", errno);
    return 1;
  }
  if(pid == 0){
    int ret;
    if(n==0)
       ret = execve("./pal", argv,NULL);
    else
       ret = execve("./facto", argv,NULL);
    if(ret < 0)
      fprintf(stderr, "Le programme n'a pas pu être exécuté (%d)\n", errno);
  }else{
    int status;
    pid = wait(&status);
  }
  return 0;
}
