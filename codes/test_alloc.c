#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
   /** erreurs fréquentes lors de l'utilisation d'un pointeur**/
   // erreur 1 Utiliser un pointeur sans l'avoir initialiser
   // Cela produit un erreur de mémoire
   int *pi;
   *(pi) = 2;

   // erreur 2, accéder à la mémoire au delà de l'espace mémoire
   // alloué
   double *p;
   int i;
   p = (double*) malloc(sizeof(double));
   printf("valeur contenue à l'adresse p :%f \n",*p);
   for(i = 1; i < 8192000; i++)
      printf("valeur se trouvant à l'adresse p+%d %f\n",i, *(p+i));

   // Erreur 3 finir d'utiliser la mémoire et oublier de la libérer
   free(p);
   return 0;
}
