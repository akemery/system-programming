#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handle_sigsegv(int number){
  fprintf(stderr, "\n%ld a recu le signal %d (%s)\n", 
    (long) getpid(), number, strsignal(number));        
}

int main(int argc, char *argv[]){
  char *buffer;
  if (signal(SIGALRM, handle_sigsegv) == SIG_ERR)
    fprintf(stderr, "Signal %d non capture\n", SIGSEGV);
  memcpy(buffer, "Bonjour", 10);
  return 0;
}
