//https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *test_pal(void *ptr);
void *test_facto(void *ptr);

int main(int argc, char*argv[]){
   pthread_t thread1, thread2;
   if(argc!=3){
      printf("Usage : %s <string> <int>\n", argv[0]);
      return -1;
   }
   int n = atoi(argv[2]);
   int  iret1, iret2;

   /* Create independent threads each of which will execute function */

   iret1 = pthread_create( &thread1, NULL, test_facto, (void*)&n);
   iret2 = pthread_create( &thread2, NULL, test_pal, (void*) argv[1]);

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 

   printf("\n\nThread 1 returns: %d\n",iret1);
   printf("Thread 2 returns: %d\n",iret2);
   exit(0);
}


void *test_pal(void *ptr){
  char *str = (char*)ptr;
  int i,j, k = 1;
  if(str==NULL)
    k = -1;
  for (i = 0, j = strlen (str) - 1; i <= j; ++i, --j){
     while(str[i]==' ')i++;
     while(str[j]==' ')j--;
     if(i>j) break;
     if (str [i] != str[j]) {
         k= 0;
         break;
     }
  }
  if(k)
     printf("%s est un palindrome\n", str);
  else
     printf("%s n'est pas un pelendrome\n", str);
}

int facto(int n){
  if(n==0)
    return 1;
  else 
    return n*facto(n-1);
}

void *test_facto(void *ptr){
  int *n = (int*)ptr;
  int res;
  res = facto(*n);
  printf("Le factoriel(%d) = %d\n", *n, res);
}


