#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"
#include <stdint.h>

#define reduce_avx KYBER_NAMESPACE(_reduce_avx)
int16_t reduce_avx(int16_t *r, const int16_t *qdata);
#define csubq_avx KYBER_NAMESPACE(_csubq_avx)
int16_t csubq_avx(int16_t *r, const int16_t *qdata);
#define tomont_avx KYBER_NAMESPACE(_tomont_avx)
int16_t tomont_avx(int16_t *r, const int16_t *qdata);

#endif
