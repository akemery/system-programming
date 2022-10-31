#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define BUF_SIZE 1024
int main(int argc, char *argv[]){
  if(argc !=2){
     fprintf(stderr, "USAGE %s <file_pathname>\n", argv[0]);
     return 0;
  }
  char *filename = argv[1], buffer[BUF_SIZE];
  int ret, fd;
  fd = open(filename, O_RDONLY);
  if(fd < 0){
    fprintf(stderr, "Erreur d'ouverture du fichier %d\n", errno);
    return 1;
  }
  while((ret = read(fd, buffer, BUF_SIZE))>0){
     fprintf(stderr, "%s", buffer);
  }
  fprintf(stderr, "Je suis le programme (%d)\n", getpid());
  return 0;
}
