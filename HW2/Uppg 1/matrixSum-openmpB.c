/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

double start_time, end_time;
int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

typedef struct {
	long value;
	long i;
	long j;
} Index;

Index globalMin;
Index globalMax;

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, total=0;
  globalMin.value = 100;
  globalMax.value = 0;
  Index localMin, localMax;

  localMin.value = 100;
  localMax.value = 0;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;



  omp_set_num_threads(numWorkers);
  srand(time(NULL));
  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
	  }
  }

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++)
  {
    printf("[ ");
    for (j = 0; j < size; j++)
    {
      printf(" %d", matrix[i][j]);
    }
    printf(" ]\n");
  }
#endif

  start_time = omp_get_wtime();
#pragma omp parallel for reduction (+:total) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];

      if (matrix[i][j] < localMin.value) {
        #pragma omp critical(updateMin)
        {
          if (matrix[i][j] < localMin.value) {
            localMin.value = matrix[i][j];
            localMin.i = i;
            localMin.j = j;
          }
        }
      }
      if (matrix[i][j] > localMax.value) {
        #pragma omp critical(updateMin)
        {
          if (matrix[i][j] > localMax.value) {
            localMax.value = matrix[i][j];
            localMax.i = i;
            localMax.j = j;
        }
      }
    }
  }


// implicit barrier
  #pragma omp master
  {
  end_time = omp_get_wtime();

  printf("the total is %d\nMax = %d (%ld, %ld)\nMin = %d (%ld, %ld)\n", total, localMax.value, localMax.j, localMax.i, localMin.value, localMin.j, localMin.i);
  printf("it took %g seconds\n", end_time - start_time);
  }
}
