#include <stdlib.h>
struct node{
  int val;
  struct node *next;
};

struct node *init_node(void);
struct node *addHead(struct node *head);
void addTail(struct node *head);
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
