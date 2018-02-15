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
#define MAXBIRDS 10 /* maximum number of workers */

sem_t dishEmpty, dishFull, criticalSec;

int numBirds;          /* number of workers */
int worms;

void *Bird(void *);
void *MamaBird(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
	worms = 0;
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t workerid[MAXBIRDS+1];

	sem_init(&dishFull, 0, 0);
	sem_init(&dishEmpty, 0, 0);
	sem_init(&criticalSec, 0, 1);


	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);


	/* read command line args if any */
	numBirds = (argc > 1) ? atoi(argv[1]) : MAXBIRDS;
	if (numBirds > MAXBIRDS)
	numBirds = MAXBIRDS;

	/* do the parallel work: create the workers */
	for (l = 0; l < numBirds; l++) {
		pthread_create(&workerid[l], &attr, Bird, (void *)l);
	}
	pthread_create(&workerid[numBirds], &attr, MamaBird, NULL);
	for (l = 0; l < numBirds; l++) {
		pthread_join(workerid[l], NULL);
	}
	pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
After a barrier, worker(0) computes and prints the total */
void *Bird(void *arg){
	long myid = (long)arg;

	while (true) {
    sleep(1);
		sem_wait(&criticalSec);
		if (worms == 0) {
			sem_post(&dishEmpty);
			printf("Bird %ld woke MamaBird becasue there is no more food\n", myid);
			sem_wait(&dishFull);
		}
    worms--;
    printf("Bird %ld ate a worm, current worms is %d\n", myid, worms);
		sem_post(&criticalSec);
	}

	pthread_exit(NULL);
}

void *MamaBird(void *arg) {
	while (true) {
		sem_wait(&dishEmpty);
    printf("MamaBird flew away to get food");
    sleep(3);
    srand(time(NULL));
		worms = 1 + rand() % 49;
		printf("MamaBird brought food, current amount of worms is %d\n", worms);
		sem_post(&dishFull);
	}
pthread_exit(NULL);
}
