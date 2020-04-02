#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"
#include <stdint.h>

#define reduce_avx KYBER_NAMESPACE(reduce_avx)
int16_t reduce_avx(int16_t *r, const uint16_t *qdata);
#define csubq_avx KYBER_NAMESPACE(csubq_avx)
int16_t csubq_avx(int16_t *r, const uint16_t *qdata);
#define tomont_avx KYBER_NAMESPACE(tomont_avx)
int16_t tomont_avx(int16_t *r, const uint16_t *qdata);

#endif
