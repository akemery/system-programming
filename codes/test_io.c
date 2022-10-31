#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]){
   int ret, nchars = 0;
   // configuration de la sortie standard en non bloquant
   fcntl(1, F_SETFL, O_NONBLOCK );
   
   // Debut de l'écriture
   while(1){
       ret = write(1, "Bonjour  ", 10 );
       if(ret < 0){
          if(errno == EAGAIN){
             printf("Buffer is full: %d\n", nchars);
             break;
          }
       }
       nchars+=ret;
   }
}
