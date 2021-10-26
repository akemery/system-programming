#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char *argv[]){
  struct stat statbuf;
  if(argc != 2){
    fprintf(stderr, "Usage: %s <nom du fichier>\n", argv[0]);
    return 0;
  }
  char *dname = argv[1] ;
  if(stat(dname, &statbuf) < 0){
    fprintf(stderr, "Impossible d'obtenir l'état du fichier %s (%s)\n"
      , dname, strerror(errno));
    return 1;
  }
  if(statbuf.st_mode & S_IFDIR){
    fprintf(stderr, "%s est un répertoire : ", dname);
    fprintf(stderr, "Dernière modification à %s\n", ctime(&statbuf.st_mtime));
    mkdir(dname, 0777);
  }
  else{
    fprintf(stdout, "%s n'est pas un répertoire\n", dname);
  }
  return 0;
}
