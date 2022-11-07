#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <arpa/inet.h>

int init();

struct message{
  long source;
  long dest;
  long opcode;
};

int main(int argc, char *argv[]){

   int ret, size_inbytes = 0, sd, csd;
   fd_set readset, writeset;
   struct sockaddr client_addr;
   int clen;
   sd = init();
   /*fcntl(1, F_SETFL, O_NONBLOCK );
   fcntl(sd, F_SETFL, O_NONBLOCK );
   FD_ZERO(&readset);
   FD_ZERO(&writeset);*/
   
   if((csd = accept(sd, &client_addr, &clen)) < 0){
      fprintf(stderr, "Un petit problème lors du accept %d\n", errno);
      return -1;
   }
   fprintf(stdout, "tentative de connexion\n");
   write(csd, "Bonjour tous le monde", 21);
   for(;;){}
   while (1){
       //FD_SET(sd, &writeset);
       /*FD_SET(sd, &readset);
       ret = select(sd+1, &readset, &writeset, NULL, NULL);
       if(ret < 0){
          printf("erreur de select\n");
          exit(1);
       }
       if (FD_ISSET(sd, &readset)){
          fprintf(stdout, "tentative de connexion\n");
          FD_CLR(sd, &readset);
          break;
       }*/
       fprintf(stdout, "tentative de connexion\n");
    }
    fprintf(stderr , "La taille du buffer du clavier est %d", size_inbytes);
}

int init(){
   int sd, ret;
   struct sockaddr_in serv_addr;
   sd = socket(AF_INET, SOCK_STREAM, 0);
   if(sd<0){
      printf("Error in socket creation\n");
      return sd;
   }
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(5555);

   ret=bind(sd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
   if(ret<0){
      printf("Error in bind\n");
      close(sd);
      return ret;
    }

    if(listen(sd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }
    
    return sd;

}
