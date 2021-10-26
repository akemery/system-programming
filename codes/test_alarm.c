#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handle_alarm(int number){
  fprintf(stderr, "\n%ld a recu le signal %d (%s)\n", 
    (long) getpid(), number, strsignal(number));
  alarm(5);	        
}

int main(int argc, char *argv[]){
  if (signal(SIGALRM, handle_alarm) == SIG_ERR)
    fprintf(stderr, "Signal %d non capture\n", SIGALRM);
  alarm(5);
  while(1){
    char c;
    fprintf(stdout,"Entrez un caractère svp ou N ou terminer\n");
    scanf("%c", &c);
    fprintf(stdout, "Vous venez de saisir (%c)\n", c);
    if(c == 'N')
      break;
  }
  pause();
  return 0;
}
