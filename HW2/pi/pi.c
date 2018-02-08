#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 1000000000 /* maximum matrix size */
#define MAXWORKERS 10 /* maximum number of workers */

int numWorkers;
static long intervals = 0;
long double step;
 void main(int argc, const char *argv[]){

   intervals = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
   numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;

   if (intervals > MAXSIZE)
   intervals = MAXSIZE;
   if (numWorkers > MAXWORKERS)
   numWorkers = MAXWORKERS;

  int i;
  long double x, sum = 0.0;
  long double pi = 0.0;
  step = 1.0/(double)intervals;
  omp_set_num_threads(numWorkers);
  double start = omp_get_wtime();

  #pragma omp parallel for private(x) reduction(+:sum)
  for (i = 0; i < intervals; i++) {
    x = (i + 0.5)*step;
    sum += 4.0/(1.0+x*x);
  }
  pi = step *= sum;
  double runtime = omp_get_wtime() - start;
  printf("Estimation of pi is %32.30Lf \n", pi);
  printf("(Actual pi value is 3.141592653589793238462643383279...)\n");
  printf("Computed in = %g", runtime);
  }
