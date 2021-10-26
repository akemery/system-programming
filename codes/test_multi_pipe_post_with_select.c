#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[]){
  int i;
  pid_t pid[4];
  int write_pipe[4][2], ret;
  int read_pipe[4][2];
  for(i = 0; i < 4; i++){
    ret = pipe(write_pipe[i]);
    if(ret < 0){
      fprintf(stderr,"Erreur de création du pipe write (%d)\n", errno);
      return 1;
    }
    
    ret = pipe(read_pipe[i]);
    if(ret < 0){
      fprintf(stderr,"Erreur de création du pipe read (%d)\n", errno);
      return 1;
    }
    
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
      ret = close(write_pipe[i][1]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité write_pipe[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      ret = close(read_pipe[i][0]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité read_pipe[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      while(1){
        int ret = read(write_pipe[i][0], buffer, 1);
        if(ret < 0){
          fprintf(stderr, "Fils (%d) impossible de lire sur write_pipe[0] du pipe (%d)\n", getpid(), errno);
          return 1;
        }
        fprintf(stdout, "<%s>, (%d)\n", buffer, getpid());
        
        char write_buffer[10];
        sprintf(write_buffer, "(%d):(%s)\n", getpid(), buffer);
        write(read_pipe[i][1], write_buffer, 10);
        if(strcmp(buffer, "N")==0)
          break;
      }
      ret = close(write_pipe[i][0]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité write_pipe[0] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      
      ret = close(read_pipe[i][1]);
      if(ret < 0){
        fprintf(stderr, "Fils (%d) impossible de fermer l'extrémité read_pipe[1] du pipe (%d)\n", getpid(), errno);
        return 1;
      }
      return 0;
    }
    else{
   
    }
  }
  for(i = 0; i < 4; i++){
    ret = close(write_pipe[i][0]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité write_pipe[0] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
    ret = close(read_pipe[i][1]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité read_pipe[0] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
  }
  char carac;
  char strcarac[2];
  char readbuffer[10];
  fd_set readset;
  fd_set writeset;
  int maxfd;
  while(1){
    FD_ZERO(&readset);
    FD_SET(0, &readset);
    for(i = 0; i < 4; i ++){
      FD_SET(read_pipe[i][0], &readset);
      maxfd = read_pipe[i][0];
    }
    
    //scanf("%c", &carac);
    if((select(maxfd+1, &readset, &writeset, NULL, NULL)< 0) && (errno != EINTR)){
      fprintf(stderr, "select a échoué %s\n", strerror(errno));
    }
    for(i = 0; i < 4; i ++){
      if(FD_ISSET(read_pipe[i][0], &readset)){
        read(read_pipe[i][0], readbuffer, 10);
        fprintf(stdout, "%s\n", readbuffer);
      }
    }
    if(FD_ISSET(0, &readset)){
      scanf("%c", &carac);
      if(carac == '\n')
        continue;
      sprintf(strcarac, "%c", carac);
      printf("(%s)\n",strcarac);
      for(i = 0; i < 4; i++){
        ret = write(write_pipe[i][1], strcarac, 1);
        if(ret < 0){
          fprintf(stdout, "Père (%d) impossible d'écrire sur fd[1] du pipe (%d)\n", getpid(), errno);
          return 1;
        }
      }
      if(carac == 'N')
        break;
    }
  }
  for(i = 0; i < 4; i++){
    ret = close(write_pipe[i][1]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité fd[1] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
    
    ret = close(read_pipe[i][0]);
    if(ret < 0){
      fprintf(stderr, "Père (%d) impossible de fermer l'extrémité fd[1] du pipe (%d)\n", getpid(), errno);
      return 1;
    }
  }
  for(i = 0; i < 4; i++){
      int status;
      wait(&status);
  }
}
