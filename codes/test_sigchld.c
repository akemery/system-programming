#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_sigchld(int number){
  int status;
  fprintf(stderr, "\n%ld a recu le signal %d (%s)\n", 
    (long) getpid(), number, strsignal(number)); 
  pid_t pid = wait(&status);
  fprintf(stderr, "\Mon fils (%d) vient de se terminer (%d)\n", pid, status); 	
         
}

int main(int argc, char *argv[]){
  char *buffer;
  if (signal(SIGALRM, handle_sigchld) == SIG_ERR)
    fprintf(stderr, "Signal %d non capture\n", SIGSEGV);
  memcpy(buffer, "Bonjour", 10);
  return 0;
}
