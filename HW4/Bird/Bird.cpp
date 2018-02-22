#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <iostream>
#include <pthread.h>
#include "monitor.h"
#include <stdbool.h>
#include <unistd.h>

#define  MAX_BIRDS 10
#define MAX_TIMES 10

void * MamaBird(void *);
void * bird(void *);

int numBirds, times_refiled;

bool done;

monitor * mon;

int main(int argc, char ** argv) {
  pthread_t animals[MAX_BIRDS + 1];
  pthread_attr_t attr;

  long index;
  times_refiled = 0;
  done = false;

  if (argc < 2) {
    std::cout << "Usage ./Bird birds" << std::endl;
    return EXIT_FAILURE;
  }
  else{
    numBirds = (atoi(argv[1]) > MAX_BIRDS) ? MAX_BIRDS : atoi(argv[1]);
  }
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  srand(time(NULL));
  mon = new monitor();

  for (index = 0; index < numBirds; index++) {
    pthread_create(&animals[index], &attr, bird, (void *) index);
  }
  pthread_create(&animals[numBirds], &attr, MamaBird, NULL);
  for (int i = 0; i < numBirds + 1; i++) {
    pthread_join(animals[i], NULL);
  }
  delete mon;
  return EXIT_SUCCESS;
}

void * bird(void * input) {
  long number = (long) input;
  while (done == false) {
    sleep(1);
    if (done) {
      pthread_exit(NULL);
    }
    mon->eat_worm(number);
    if (done) {
      pthread_exit(NULL);
    }
  }
  pthread_exit(NULL);
}

void * MamaBird(void * input){
  while (true) {
    mon->fill_dish();
    times_refiled++;
    std::cout << "MamaBird refilled the dish" << times_refiled << " out of " << MAX_TIMES << " times." << std::endl;
    if (times_refiled >= MAX_TIMES) {
      done = true;
      pthread_exit(NULL);
    }
  }
}
