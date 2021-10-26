#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
  char *current_dir;
  long maxpath;
  if((maxpath= pathconf("/", _PC_PATH_MAX))==-1){
    fprintf(stderr, "Erreur de déterminaison du maxpath (%d)\n", errno);
    return 1;
  }
  current_dir = (char*) malloc(maxpath+1);
  if(!current_dir){
    fprintf(stderr, "Erreur d'allocation de mémoire (%d)\n", errno);
    return 1;
  }
  char *path = getcwd(current_dir, maxpath);
  if(path == NULL){
    fprintf(stderr, "Impossible d'avoir le répertoire courrant (%d)\n", errno);
    return 1;
  }
  fprintf(stdout, "%s\n", current_dir);
  return 0;
}
