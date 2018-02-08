/* parallel program to calculate pi.
 * Since PI == 4 * arctan(1), and arctan(x) is the
 *  integral from 0 to x of (1/(1+x*x),
 *  the for loop below approximates that integration.
 * Usage: ./a.out <numIntervals> <numWorkers>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

/* global variables */
long double pi = 0.0; /* the nuber of digits for the aprox */
pthread_mutex_t lock;        /* how we synchronize writes to 'pi' */
long double intervals;         /* how many intervals for the integration */
int numWorkers;                /* the number of threads */

/* the function a thread executes
 * Parameters: a void* storing the address of the thread ID.
 */
void *calculatePi(void *id){
    long double x, width, localSum = 0;
    int i, threadID = *((int*)id);
    width = 1.0 / intervals;

    for(i = threadID ; i < intervals; i += numWorkers) {
        x = (i+0.5) * width;
        localSum += 4.0 / (1.0 + x*x);
    }

    localSum *= width;

    pthread_mutex_lock(&lock);
    pi += localSum;
    pthread_mutex_unlock(&lock);

    return NULL;
}
/* the timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized ){
        gettimeofday(&start, NULL);
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

int main(int argc, char **argv){
    pthread_t *threads;
    int *threadID;
    int i;
    double starttime, endtime;

   if (argc == 3) {
      intervals = atoi(argv[1]);
      numWorkers = atoi(argv[2]);
      threads = malloc(numWorkers*sizeof(pthread_t));
      threadID = malloc(numWorkers*sizeof(int));
      pthread_mutex_init(&lock, NULL);

        starttime = read_timer(); // Reads the clock the first time.

      for (i = 0; i < numWorkers; i++) {
        threadID[i] = i;
        pthread_create(&threads[i], NULL, calculatePi, threadID+i);
      }

      for (i = 0; i < numWorkers; i++) {
         pthread_join(threads[i], NULL);
      }
        endtime = read_timer(); // Reads the clock the second time.

      printf("Estimation of pi is %32.30Lf \n", pi);
      printf("(Actual pi value is 3.141592653589793238462643383279...)\n");
      printf("Execution time: %g\n", endtime - starttime );
    }
    else {
      printf("Usage: ./a.out <numIntervals> <numWorkers>");
    }

    return 0;
}
