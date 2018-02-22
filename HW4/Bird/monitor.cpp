#include "monitor.h"

monitor::monitor(){

  pthread_mutex_init(&dishLock, NULL);
  pthread_cond_init(&dishEmpty, NULL);
  pthread_cond_init(&dishRefilled, NULL);

  wormCount = 5;
}
monitor::~monitor(){
}

void monitor::fill_dish(){
  pthread_cond_wait(&dishEmpty, &dishLock);
  std::cout << "Mama bird flew away to get food" << std::endl;
  wormCount = 1 + rand() % MAX_WORMS;
  std::cout << "MamaBird brought food, current amount of worms is " << wormCount << std::endl;
  pthread_cond_broadcast(&dishRefilled);
}

void monitor::eat_worm(long number) {
  pthread_mutex_lock(&dishLock);
  if (wormCount > 0) {
    wormCount--;
    std::cout << "bird " << number <<" ate a worm " << wormCount << std::endl;
    while (wormCount == 0) {

      std::cout << "bird discoverd that dish was empty" << std::endl;
      pthread_cond_broadcast(&dishEmpty);
      pthread_cond_wait(&dishRefilled, &dishLock);
    }
  }
  pthread_mutex_unlock(&dishLock);
}
