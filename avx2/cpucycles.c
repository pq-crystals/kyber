#include "cpucycles.h"

unsigned long long cpucycles_overhead(void) {
  unsigned long long t0, t1, overhead = -1;
  unsigned int i;

  for(i = 0; i < 100000; ++i) {
    t0 = cpucycles_start();
    asm volatile("");
    t1 = cpucycles_stop();
    if(t1 - t0 < overhead)
      overhead = t1 - t0;
  }

  return overhead;
}
