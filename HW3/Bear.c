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
#define MAXWORKERS 10 /* maximum number of workers */

pthread_mutex_t lock;   /* Lock for  */
int numbees;          /* number of workers */


/* timer */
double read_timer()
{
	static bool initialized = false;
	static struct timeval start;
	struct timeval end;
	if (!initialized)
	{
		gettimeofday(&start, NULL);
		initialized = true;
	}
	gettimeofday(&end, NULL);
	return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time;  /* start and end times */


void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[])
{
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t workerid[MAXWORKERS];


	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/* initialize mutex and condition variable */
	pthread_mutex_init(&lock, NULL);

	/* read command line args if any */
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE)
	size = MAXSIZE;
	if (numWorkers > MAXWORKERS)
	numWorkers = MAXWORKERS;

	/* do the parallel work: create the workers */
	start_time = read_timer();
	for (l = 0; l < numWorkers; l++) {
		pthread_create(&workerid[l], &attr, Worker, (void *)l);
	}
	for (l = 0; l < numWorkers; l++) {
		pthread_join(workerid[l], NULL);
	}
	//get end time
	end_time = read_timer();

	// print result
	/* print results */
	printf("The execution time is %g sec\n", end_time - start_time);

	pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
	long myid = (long)arg;

	#ifdef DEBUG
	printf("worker %ld (pthread id %ld) has started\n", myid, (long)pthread_self());
	#endif



	#ifdef DEBUG
	printf("worker %ld (pthread id %ld) has finished\n", myid, (long)pthread_self());
	#endif

	pthread_exit(NULL);
}
