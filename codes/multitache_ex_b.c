#include <string.h>
#include <stdio.h>

#define REPONSE_SIZE 30
int main(int argc, char *argv[]){
   char requete;
   char reponse[REPONSE_SIZE];
   /* reception de la requête */
   scanf("%c", &requete);
   
   /*traitement de la requête */
   switch(requete){
     case 'a':
       strcpy(reponse, "BENIN");
       break;
     case 'b':
       strcpy(reponse, "TOGO");
       break;
     case 'c':
       strcpy(reponse, "GHANA");
       break;
     default:
       strcpy(reponse, "REQUETE NON PRISE EN CHARGE");
       break;
   }
   
   /*Envoie de la réponse*/
   printf("%s\n", reponse);
   return 0;
}
