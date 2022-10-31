#include <stdio.h>
#include <stdlib.h>
#define TAB_SIZE 1024

void init(int times);

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("USAGE: %s <times>", argv[0]);*
    return 0;
  }
  init(atoi(argv[1]));
  return 0;
}

void init(int times){
  int tab[TAB_SIZE*times];
  for(int i = 0; i < TAB_SIZE*times; i++)
    tab[i] = i;
}
