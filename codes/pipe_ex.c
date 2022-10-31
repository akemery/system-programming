#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
  int fd[2];
  int err;
  err = pipe(fd);
  if(err < 0){
    fprintf(stderr, "Erreur de création du pipe %d %d\n", err, errno);
    return 1;
  }
  pid_t pid = fork();
  if(pid < 0){
    fprintf(stderr, "Erreur de création du processus fils %d %d\n", pid, errno);
    return 1;
  }
  
  if(pid == 0){
    char *buffer_fils = "Bonjour mon papa processus. Si j'étais un humain je t'aurais bien aimé. Mais malheureusement je ne suis qu'un processus. Bof.";
    close(fd[0]);
    write(fd[1], buffer_fils, 125);
  }
  else{
    char buffer_pere[126];
    close(fd[1]);
    read(fd[0], buffer_pere, 125);
    printf("Je viens de recevoir un joli message de mon fils ### %s ####\n", buffer_pere);
    int status;
    pid = wait(&status);
  }
  return 0;
}
