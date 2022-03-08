#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]){
  if(argc != 3){
     printf("Usage: %s <arg1> <arg2>", argv[0]);
     exit(0);
  }
  int fd1, fd2, rc;
  char buf;
  fd1 = open(argv[1], O_RDONLY);
  if(fd1 < 0){
     perror("open(fd1)");
     exit(1);
  }
  fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
  if(fd2 < 0){
     perror("open(fd2)");
     close(fd1);
     exit(2);
  }
  while(1){
     rc = read(fd1, &buf, 1);
     if(rc < 0){
        perror("read");
	exit(3);
     }
     if(rc == 0)
        break;
     rc = write(fd2, &buf, 1);
     if(rc < 0){
        perror("write");
	close(fd1);
	close(fd2);
	exit(4);
     }
  }
  close(fd1);
  close(fd2);
  return 0; 
}
