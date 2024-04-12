#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 8192

int init();

struct message{
  long source;
  long dest;
  long opcode;
};

int main(int argc, char *argv[]){

   int ret, wret, size_inbytes = 0, sd;
   fd_set readset, writeset;
   
   if(argc < 2){
       fprintf(stderr, "%s <dest_file_name>\n", argv[0]);
       return 1;
   }
   sd = init();
   fcntl(1, F_SETFL, O_NONBLOCK );
   fcntl(sd, F_SETFL, O_NONBLOCK );
   FD_ZERO(&readset);
   FD_ZERO(&writeset);
   char buffer[BUFFER_SIZE];
   int fd = open(argv[1], O_WRONLY | O_CREAT , S_IRWXU);
   if(fd < 0){
       fprintf(stderr, "Unable to create the destination file\n");
       return 0;
   }
   while (1){
       FD_SET(sd, &writeset);
       FD_SET(sd, &readset);
       ret = select(sd+1, &readset, &writeset, NULL, NULL);
       if (FD_ISSET(sd, &readset)){
          ret = read(sd, buffer, BUFFER_SIZE);
          wret = write(fd, buffer, ret); 
          FD_CLR(sd, &readset);
       }
    }
    fprintf(stderr , "La taille du buffer du clavier est %d", size_inbytes);
}

int init(){
   struct sockaddr_in serv_addr;
   int sd;
   /* Creation du socket*/
   if((sd = socket(AF_INET, SOCK_STREAM, 0))< 0){
        printf("\n Erreur  de socket \n");
        return sd;
   }

   /* Initialisation du strucutre du socket */
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(4445); // port
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    
   if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        printf("\n Error: Connect Failed (%d) \n", errno);
        return -1;
   }
   
   return sd;
}
