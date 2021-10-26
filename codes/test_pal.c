#include <stdio.h>
#include <string.h>

int pal(char *str){
  int i,j, k = 1;
  if(str==NULL)
    return -1;
  for (i = 0, j = strlen (str) - 1; i <= j; ++i, --j){
     while(str[i]==' ')i++;
     while(str[j]==' ')j--;
     if(i>j) break;
     if (str [i] != str[j]) {
         k= 0;
         break;
     }
  }
  return k;
}

int main(int argc, char *argv[]){
  if(argc!=2){
    fprintf(stderr, "USAGE: %s <chaine de caractere> \n", argv[0]);
    return 1;
  }
  char *str = argv[1];
  if(pal(str))
    fprintf(stderr, "%s est un palindrome\n", str);
  else
    fprintf(stderr, "%s n'est pas un palindrome\n", str);
  return 0;
}
