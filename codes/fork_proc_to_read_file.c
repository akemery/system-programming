#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  char *buffer = "<!doctype html> <html>" ;
  int fd = open(argv[1], O_CREAT | S_IRWXU | O_RDWR);
  if(fd < 0){
    fprintf(stderr, "Erreur d'ouverture du fichier %d %d\n", fd, errno);
    return 1;
  }
  int ret = write(fd, buffer, 22);
  if(ret < 0){
    fprintf(stderr, "Erreur d'écriture %d %d %s %ld %d\n", ret, errno, buffer, sizeof(buffer), fd);
    close(fd);
    return 1;
  }
  close(fd);
  
  fprintf(stderr, "Je suis le processu (%d)\n", getpid());
  fprintf(stderr, "Je m'apprête à créer un processus\n");
  pid_t pid = fork();
  if(pid < 0){
    fprintf(stderr, "Erreur de création du processus (%d)\n", errno);
    return 1;
  }
  if(pid == 0){
    fprintf(stderr, "Je suis le processus fils (%d) qui vient d'être créé par (%d)\n", getpid(), getppid());
    int err = execve("./read_file", argv,NULL);
  }else{
    fprintf(stderr, "Je suis le processus (%d) qui vient de créer le processus (%d)\n", getpid(), pid);
    int status;
    pid = wait(&status);
  }
  return 0;
}
