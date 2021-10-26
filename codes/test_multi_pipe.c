#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
  int i;
  pid_t pid[4];
  int fd[4][2], ret;
  ret = pipe(fd[0]);
  if(ret < 0){
    fprintf(stderr,"Erreur de création du pipe (%d)\n", errno);
    return 1;
  }
  for(i = 0; i < 4; i++){
    pid[i] = fork();
    if(pid[i]<0){
      /*On gère l'erreur*/
      fprintf(stderr, "Erreur lors de la création du fils(%d)\n", errno);
      return 1;
    }
    if(pid[i] == 0){
      char buffer[1];
      /* Dans le fils*/
      fprintf(stdout, "Je suis (%d), mon père (%d)\n", getpid(), getppid());
      ret = close(fd[i][1]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité fd[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      int ret = read(fd[i][0], buffer, 1);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de lire sur fd[0] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      fprintf(stdout, "<%s>, (%d)\n", buffer, getpid());
      return 0;
    }
    else{
   
    }
  }
  ret = close(fd[i][0]);
  if(ret < 0){
    fprintf(stderr, "Père (%d) impossible de fermer l'extrémité fd[0] du pipe (%d)\n", getpid(), errno);
    return 1;
  }
  char carac;
  char strcarac[2];
  while(1){
    scanf("%c", &carac);
    if(carac == '\n')
      continue;
    sprintf(strcarac, "%c", carac);
    ret = write(fd[i][1], strcarac, 1);
    if(ret < 0){
      fprintf(stdout, "Père (%d) impossible d'écrire sur fd[1] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
    if(carac == 'N')
      break;
  }
  for(i = 0; i < 4; i++){
      int status;
      wait(&status);
  }
}
