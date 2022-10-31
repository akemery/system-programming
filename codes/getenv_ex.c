#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  char *path = getenv("PATH");
  printf("%s\n", path);
  return 0;
}
