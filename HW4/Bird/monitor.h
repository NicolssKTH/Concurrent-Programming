#ifndef _FEED_MOMNITOR
#define _FEED_MOMNITOR

#include <pthread.h>
#include <iostream>

class monitor{
public:
  monitor();
  ~monitor();

  void fill_dish();
  void eat_worm(long number);

  const static int MAX_WORMS = 25;

private:
  pthread_cond_t dishEmpty;
  pthread_cond_t dishRefilled;
  pthread_mutex_t dishLock;

  int wormCount;
};

#endif
