#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  char *buffer = "<!doctype html> <html>" ;
  int fd = open("IG2_PARAKOU_2021.html", O_CREAT | S_IRWXU | O_RDWR);
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
  return 0;
}
