#include <stdlib.h>
struct node{
  int val;
  struct node *next;
};

/** 
   Initialise une liste vide
**/
struct node *init_node(void);

/** Ajoute la valeur à la tête de la liste et retourne 
 *  la nouvelle tête (head)
 **/
struct node *addHead(struct node *head, int val);

/** Ajoute l'élément à la fin de la liste. 
 * La tête de liste ne change pas
 **/
void addTail(struct node *head, int val);

/** 
 * alloue de la mémoire pour un nouveau noeud et retourne ce noeud
 **/
struct node *new_node(int val);

/** affiche tous les éléments de la liste 
 *  au format val: <valeur>
 **/
void display_list(struct node *head);

int main(int argc, char *argv[]){
   struct node *head;
   head = init_node();
   return 0;
}

struct node *init_node(void){
  struct node * n = (struct node*)malloc(sizeof(struct node));
  n->val = -1;
  return n;
}
