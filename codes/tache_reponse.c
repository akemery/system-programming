#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
   int fd = atoi(argv[1]);
   char reponse[25];
   char rep_len_str[3];
   int rep_len;
   /* reception de la requête */
   printf("salut %d\n", fd);
   while(1){
    int err = read(fd, rep_len_str, 2);
    rep_len_str[3] = '\0';
    sscanf(rep_len_str, "%d", &rep_len);
    err = read(fd, reponse, rep_len);
    reponse[rep_len] = '\0';
    printf("1: %s %d %d %s\n",rep_len_str, rep_len, err, reponse);
    
    //read(fd, reponse, 25);
    //printf("2: %s %s %ld %ld\n", reponse, rep_len, strlen(reponse), strlen(rep_len));
   }
}
