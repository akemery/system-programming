#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[]){
  int i;
  pid_t pid[4];
  int fd[4][2], ret;
  for(i = 0; i < 4; i++){
    ret = pipe(fd[i]); /* création du pipe pour le fils i*/
    if(ret < 0){
      fprintf(stderr,"Erreur de création du pipe (%d)\n", errno);
      return 1;
    }
    pid[i] = fork(); /* création du fils i*/
    if(pid[i]<0){
      /*On gère l'erreur*/
      fprintf(stderr, "Erreur lors de la création du fils(%d)\n", errno);
      return 1;
    }
    if(pid[i] == 0){
      char buffer[1];
      /* Dans le fils*/
      ret = close(fd[i][1]);/*fermeture de l'extrémité [1] car le fils a besoin de lire et non d'écrire*/
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité fd[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      while(1){ /* le fils est en attente des données du père */
        int ret = read(fd[i][0], buffer, 1); /* Le fils reçoit un caractère du père*/
        if(ret < 0){
          fprintf(stderr, "Fils (%d) impossible de lire sur fd[0] du pipe (%d)\n", getpid(), errno);
          return 1;
        }
        fprintf(stdout, "<%s>, (%d)\n", buffer, getpid());
        if(strcmp(buffer, "N")==0)
          break;
      }
      ret = close(fd[i][0]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité fd[0] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      return 0;
    }
    else{
   
    }
  }
  for(i = 0; i < 4; i++){
    ret = close(fd[i][0]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité fd[0] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
  }
  char carac;
  char strcarac[2];
  while(1){
    scanf("%c", &carac);
    if(carac == '\n')
      continue;
    sprintf(strcarac, "%c", carac);
    for(i = 0; i < 4; i++){ /* Envoie du caractère saisi à l'écran à chaque fils*/
      ret = write(fd[i][1], strcarac, 1);
      if(ret < 0){
        fprintf(stdout, "Père (%d) impossible d'écrire sur fd[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
    }
    if(carac == 'N')
      break;
  }
  for(i = 0; i < 4; i++){ /*libération de l'extrémité d'écriture du pipe de chaque fils*/
    ret = close(fd[i][1]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité fd[1] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
  }
  for(i = 0; i < 4; i++){ /* Le père attend que chacun de ses fils se termine*/
      int status;
      wait(&status);
  }
}
