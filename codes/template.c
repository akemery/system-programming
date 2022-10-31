#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
  if(argc!=3){
    fprintf(stderr, "%s <arg1> <arg2> \n", argv[0]);
    return 0;
  }
  int arg1 = atoi(argv[1]);
  int arg2 = atoi(argv[2]);
  char a;
  while(1){
     printf("Etes vous surs de vouloir continuer? n/y\n");
     scanf("%c", &a);
     if(a=='n')
       break;
     fprintf(stderr, "arg1(%d) arg2(%d)\n", arg1, arg2);
  }
  return 0;
}
