#ifndef _FEED_MOMNITOR
#define _FEED_MOMNITOR

#include <pthread.h>
#include <iostream>

class monitor{
public:
  monitor();
  ~monitor();

  void fill_pot(long number);
  void eat_honey();

  const static int MAX_HONEY = 30; /* Max portions of honey in pot. */

private:
  pthread_cond_t potEmpty;
  pthread_cond_t potFull;
  pthread_mutex_t potLock;

  int honey_count;
};

#endif
