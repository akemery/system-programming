#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;
static void * routine_thread (void * numerothread);
static int aleatoire (int maximum);
int main (void)
{
   int i;
   pthread_t thread;
   //thread = calloc(10, sizeof(pthread_t));
   sem_init (& semaphore, 0, 3);
   for (i = 0; i < 10; i++)
      pthread_create (&thread, NULL, routine_thread, (void *) &i);
   //for (i = 0; i < 10; i ++) 
	//	pthread_join(thread[i], NULL);
   pthread_exit (NULL);
}
static void *routine_thread (void * numero_thread)
{
     int j;
     long num = (long) numero_thread;
     for (j = 0; j < 5; j++) {
       // sem_getvalue (&semaphore, &sem);
        //fprintf (stdout, "Thread %d tentative dans portion critique sem %d \n",
       //     *((int*) numero_thread), sem);
        sem_wait (& semaphore);
        fprintf (stdout, "Thread %ld dans portion critique \n",
            num);
        sleep (aleatoire (4));
        fprintf (stdout, "Thread %ld sort de la portion critique \n",
            num);
        sem_post (& semaphore);
        sleep (aleatoire (4));
     }
     return (NULL);
}

static int aleatoire (int maximum)
{
	double d;
	d = (double) maximum * rand();
	d = d / (RAND_MAX + 1.0);
	return ((int) d);
}
