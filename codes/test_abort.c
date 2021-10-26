#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handle_abort(int number){
  fprintf(stderr, "\n%ld a recu le signal %d (%s)\n", 
	        (long) getpid(), number, strsignal(number));
}

int main(int argc, char *argv[]){
  if (signal(SIGABRT, handle_abort) == SIG_ERR)
    fprintf(stderr, "Signal %d non capture\n", SIGABRT);
  abort();
  return 0;
}
