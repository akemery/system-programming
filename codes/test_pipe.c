#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]){
  int fd[2], ret;
  pid_t pid;
  /* demande de création du pipe au système*/
  ret = pipe(fd);
  if(ret < 0){ /* Une erreur s'est produite lors de la création du pipe*/
    fprintf(stderr, "Erreur de création du pipe (%d)\n", errno);
    return 1;
  }
  pid = fork(); /* demande de création du processus fils*/
  if(pid < 0){ /* Une erreur s'est produite lors de la création du processus fils*/
    fprintf(stderr, "Erreur de création du fils(%d)\n", errno);
    return 1;
  }
  if(pid == 0){ /*Nous sommes dans le fils*/
    close(fd[1]); /* Le fils ferme l'extrémité d'écriture du pipe*/
    char buffer[10];
    while(1){ /* Le fils se met en attente de lecture des données du pipe*/
      int n = read(fd[0], buffer, 10);
      printf("Fils(%s)\n", buffer);
      if(strcmp(buffer, "N")==0) /* L'utilisateur met fin au programme*/
        break;
    }
  }
  else{
    close(fd[0]); /* Le père ferme l'extrémité de lecture du pipe*/
    char buffer[10];
    while(1){
      int n = scanf("%s", buffer); /* Le père lit une chaine de caractère saisie au clavier*/
      write(fd[1], buffer, n); /* le père écrit la chaine de caractère dans le pipe */
      if(strcmp(buffer, "N")==0) /* L'utilisateur met fin au programme*/
        break;
    }
    int status;
    int pid2 = wait(&status); /* Le père attend la fin du fils. */
  } 
}
