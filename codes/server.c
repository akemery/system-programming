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
   pid_t chld;
   if(argc < 2){
       fprintf(stderr, "%s <port>\n", argv[0]);
       return 1;
   }
   int port = atoi(argv[1]);
   if(port <= 0){
      fprintf(stderr, "Invalid value for port number\n");
      return 2;
   }
   
   sd = init(port);
   if(sd < 0){
      fprintf(stderr, "Error during server initialization\n");
      return 3;
   }
   fcntl(1, F_SETFL, O_NONBLOCK );
   fcntl(sd, F_SETFL, O_NONBLOCK );
   FD_ZERO(&readset);
   FD_ZERO(&writeset);
   while(1){
     FD_SET(sd, &readset);
     ret = select(sd+1, &readset, &writeset, NULL, NULL);
     if(ret < 0){
          printf("erreur de select\n");
          exit(1);
      }
       
   
      if((csd = accept(sd, &client_addr, &clen)) < 0){
        fprintf(stderr, "Un petit problème lors du accept %d\n", errno);
        return -1;
      }
      fprintf(stdout, "tentative de connexion\n");
      chld = fork();
      if(chld < 0){
        fprintf(stderr, "Unable to create child process\n");
        return 4;
      }
      if(chld == 0){
          write(csd, "Bonjour tous le monde", 21);
          close(csd);
          return 0;
      }else {
   
      }
   }
   /*for(;;){}
   while (1){
       //FD_SET(sd, &writeset);
       FD_SET(sd, &readset);
       ret = select(sd+1, &readset, &writeset, NULL, NULL);
       if(ret < 0){
          printf("erreur de select\n");
          exit(1);
       }
       if (FD_ISSET(sd, &readset)){
          fprintf(stdout, "tentative de connexion\n");
          FD_CLR(sd, &readset);
          break;
       }
       fprintf(stdout, "tentative de connexion\n");
    }
    fprintf(stderr , "La taille du buffer du clavier est %d", size_inbytes);*/
    close(sd);
    return 0;
}

int init(int port){
   int sd, ret;
   struct sockaddr_in serv_addr;
   sd = socket(AF_INET, SOCK_STREAM, 0);
   if(sd<0){
      printf("Error in socket creation\n");
      switch(errno){
         case EACCES:
              fprintf(stderr, "Permission to create a socket of the specified type"  
                      "and/or  protocol  isdenied.");
              break;
         case EAFNOSUPPORT:
              fprintf(stderr, "The implementation does not support the specified "
                               "address family.");
              break;
         case EINVAL:
              fprintf(stderr, "Unknown protocol, or protocol family not available "
                              "or Invalid flags in type. ");
              break;
         case EMFILE:
              fprintf(stderr, "The  per-process  limit  on  the number of open"
                              "file descriptors has been reached.");
              break;
         case ENFILE:
              fprintf(stderr, "The system-wide limit on the total number of "
                               "open files has been reached.");
              break;
         case ENOMEM:
              fprintf(stderr, "Insufficient memory is available.");
              break;
         case  EPROTONOSUPPORT:
               fprintf(stderr, "The  protocol type or the specified protocol is not"
                               " supported within this domain.");
               break;
          default:
               fprintf(stderr, "Unknown error occured");
               break;
              
      }
      return sd;
   }
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(port);

   ret=bind(sd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
   if(ret<0){
      switch(errno){
          case EACCES:
              fprintf(stderr, "The address is protected, "
                        "and the user is not the superuser.");
              break;
          case EADDRINUSE:
              fprintf(stderr, "The given address is already in use.");
              break;
          case EBADF:
              fprintf(stderr, "sockfd is not a valid file descriptor.");
              break;
          case EINVAL:
              fprintf(stderr, "The socket is already bound to an address.");
              break;
          case ENOTSOCK:
              fprintf(stderr, "The file descriptor sockfd does not refer to a socket.");
              break;
          default:
              break;
      }
      printf("Error in bind\n");
      close(sd);
      return ret;
    }

    if(listen(sd, 10) == -1)
    {
        switch(errno){
           case EADDRINUSE:
               fprintf(stderr, "Another socket is already listening on the same port.\n");
               break;
           case EBADF:
               fprintf(stderr, " The argument sockfd is not a valid file descriptor.\n");
               break;
           default:
               fprintf(stderr, "Error during listen system call\n");
               break;
        }
        
        return -1;
    }
    
    return sd;

}
