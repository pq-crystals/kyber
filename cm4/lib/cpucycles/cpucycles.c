#include "cpucycles.h"

#include <stdio.h>  /* printf */
#include <stdlib.h> /* qsort */

volatile unsigned int *DWT_CYCCNT = (unsigned int *)0xE0001004;
volatile unsigned int *DWT_CTRL = (unsigned int *)0xE0001000;
volatile unsigned int *SCB_DEMCR = (unsigned int *)0xE000EDFC;

inline unsigned int cpucycles() {
  return *DWT_CYCCNT;
}

void cpucycles_init() {
  *SCB_DEMCR = *SCB_DEMCR | 0x01000000;
  *DWT_CYCCNT = 0;            // reset the counter
  *DWT_CTRL = *DWT_CTRL | 1;  // enable the counter
}
