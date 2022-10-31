#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strcat_(char *dest, char *src){
  char *res=dest;
  while (*dest )
    ++ dest;
  while( *src){
    *dest++ = *src ++;
  }
  return res;
}
size_t strlen_(){
  size_t len = 0;
  while(*s!='\0')len++;
  return len;
}
int main(int argc , char *argv[]){
  char *src = malloc(10);
  char *dest = malloc(30);
  memcpy(src, "Bonjour", 7);
  *(src+8) = '\0';
  memcpy(dest, "Emery", 5);
  *(dest+5) = '\0';
  char *res = strcat_(dest, src);
  printf("%s\n", res);
  return 0;
}
