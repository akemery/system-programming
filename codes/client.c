#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 8192

int init();

struct message{
  int size;
  char cmd[256];
};

int main(int argc, char *argv[]){

   int ret, wret, size_inbytes = 0, sd;
   fd_set readset, writeset;
   struct message cl_cmd;
   if(argc < 2){
       fprintf(stderr, "%s <dest_file_name>\n", argv[0]);
       return 1;
   }
   sd = init();
   //sd = 5;
   fcntl(1, F_SETFL, O_NONBLOCK );
   fcntl(0, F_SETFL, O_NONBLOCK );
   fcntl(sd, F_SETFL, O_NONBLOCK );
   FD_ZERO(&readset);
   FD_ZERO(&writeset);
   char buffer[BUFFER_SIZE];
   /*int fd = open(argv[1], O_WRONLY | O_CREAT , S_IRWXU);
   if(fd < 0){
       fprintf(stderr, "Unable to create the destination file\n");
       return 0;
   }*/
   while (1){
       FD_SET(sd, &writeset);
       FD_SET(sd, &readset);
       //FD_SET(0, &readset);
       ret = select(sd+1, &readset, &writeset, NULL, NULL);
       printf("am here\n");
       if(FD_ISSET(sd, &writeset)){
          fprintf(stdout, ">");
          do{
             ret = scanf("%[^\n]", cl_cmd.cmd);
             //fgets(cl_cmd.cmd, 256, stdin);
          } while(ret==EOF && errno == EAGAIN);
          fprintf(stdout, "%s\n", cl_cmd.cmd);
          cl_cmd.size = strlen(cl_cmd.cmd);
          write(sd, &cl_cmd.size, 4);
          write(sd, cl_cmd.cmd, cl_cmd.size);
          fprintf(stdout, ">");
          FD_CLR(sd, &writeset);
        
       }
       printf("am IIII here\n");
        
       /*if (FD_ISSET(sd, &readset)){
          ret = read(sd, buffer, BUFFER_SIZE);
          wret = write(fd, buffer, ret); 
          if(wret < ret){
              fprintf(stderr, "aha une différence de taille\n");
          }
          FD_CLR(sd, &readset);
       }*/
       FD_CLR(0, &readset);
       FD_CLR(sd, &readset);
      
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
   serv_addr.sin_port = htons(9445); // port
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    
   if(connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        printf("\n Error: Connect Failed (%d) \n", errno);
        return -1;
   }
   
   return sd;
}
