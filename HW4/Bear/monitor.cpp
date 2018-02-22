#include "monitor.h"

monitor::monitor(){

  pthread_mutex_init(&potLock, NULL);
  pthread_cond_init(&potEmpty, NULL);
  pthread_cond_init(&potFull, NULL);

  honey_count = 0;
}
monitor::~monitor(){
}

void monitor::fill_pot(long number) {
  pthread_mutex_lock(&potLock);
  if (honey_count < MAX_HONEY) {
    honey_count++;
    std::cout << ": Bee " << number << " fills the pot (" << honey_count << "/" << MAX_HONEY << ")." << std::endl;
    while (honey_count == MAX_HONEY){
      pthread_cond_broadcast(&potFull);
      std::cout << ": Bee " << number << " wakes the bear and waits for the pot to be empty." << std::endl;
      pthread_cond_wait(&potEmpty, &potLock);
    }
  }
pthread_mutex_unlock(&potLock);
}

void monitor::eat_honey(){
  pthread_mutex_lock(&potLock);
  if(honey_count < MAX_HONEY){
    while(honey_count < MAX_HONEY){
      pthread_cond_wait(&potFull, &potLock);
    }
  }
  honey_count = 0;
  std::cout << std::endl << ": Bear ate all the honey!" << std::endl << std::endl;
  pthread_cond_broadcast(&potEmpty);
  pthread_mutex_unlock(&potLock);
}
