int main(int argc, char *argv[]){
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
}
