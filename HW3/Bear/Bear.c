#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <semaphore.h>
#define MAXBEES 10 /* maximum number of workers */
#define POTSIZE 50 /* maximum number of honey */

sem_t potEmpty, potFull, criticalSec;

int numBees;          /* number of workers */
int pot;
int size;          /* assume size is multiple of numWorkers */


void *Bee(void *);
void *Bear(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
	pot = 0;
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t workerid[MAXBEES+1];

	sem_init(&potFull, 0, 0);
	sem_init(&potEmpty, 0, 0);
	sem_init(&criticalSec, 0, 1);


	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);


	/* read command line args if any */
	size = (argc > 1) ? atoi(argv[1]) : POTSIZE;
	numBees = (argc > 2) ? atoi(argv[2]) : MAXBEES;
	if (size > POTSIZE)
	size = POTSIZE;
	if (numBees > MAXBEES)
	numBees = MAXBEES;

	/* do the parallel work: create the workers */
	for (l = 0; l < numBees; l++) {
		pthread_create(&workerid[l], &attr, Bee, (void *)l);
	}
	pthread_create(&workerid[numBees], &attr, Bear, NULL);
	for (l = 0; l < numBees; l++) {
		pthread_join(workerid[l], NULL);
	}

	pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
After a barrier, worker(0) computes and prints the total */
void *Bee(void *arg){
	long myid = (long)arg;

	while (true) {
		sleep(1);
		sem_wait(&criticalSec);
		pot++;
		printf("Bee %ld added honey, current pot is %d\n", myid, pot);
		if (pot >= size) {
			sem_post(&potFull);
			printf("Bee %ld filled the pot and woke the bear\n", myid);
			sem_wait(&potEmpty);
		}
		sem_post(&criticalSec);
	}


	pthread_exit(NULL);
}

void *Bear(void *arg) {
	while (true) {
		sem_wait(&potFull);
		pot = 0;
		printf("Bear ate all honey, current pot is %d\n", pot);
		sem_post(&potEmpty);
	}
pthread_exit(NULL);
}
