#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
  if(argc!=2){
    fprintf(stderr, "Usage: %s <nom_du_repertoire>\n", argv[0]);
    return 1;
  }
  char *dirname = argv[1];
  int ret;
  ret = mkdir(dirname, 0777);
  if(ret < 0){
    fprintf(stderr, "Impossible de créer le répertoire (%s)", strerror(errno));
    return 1;
  }
  return 0;
}
