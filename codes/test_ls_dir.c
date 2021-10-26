#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]){
  DIR *dirp;
  struct dirent *direntp;
  if(argc!=2){
    fprintf(stderr, "USAGE: %s <dir_name>\n", argv[0]);
    return 1;
  }
  char *dirname = argv[1];
  dirp = opendir(dirname);
  if(dirp == NULL){
    fprintf(stderr, "Impossible d'ouvrir le répertoire (%s)\n",strerror(errno));
    return 1;
  }
  while((direntp = readdir(dirp))!=NULL)
    fprintf(stdout, "%s\n", direntp->d_name);
}
