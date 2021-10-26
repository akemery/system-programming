#include <stdio.h>
#include <stdlib.h>

int facto(int n){
  if(n==0)
    return 1;
  else 
    return n*facto(n-1);
}

int main(int argc, char *argv[]){
   if(argc!=2){
     fprintf(stderr, "USAGE: %s <entier>\n", argv[0]);
     return 1;
   }
   int n = atoi(argv[1]);
   fprintf(stderr, "Le factoriel de %d est %d\n", n, facto(n));
   return 0;
}
