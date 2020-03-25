#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"

#define reduce_avx KYBER_NAMESPACE(reduce_avx)
#define csubq_avx KYBER_NAMESPACE(csubq_avx)
#define tomont_avx KYBER_NAMESPACE(tomont_avx)

#ifndef __ASSEMBLER__
#include <stdint.h>
int16_t reduce_avx(int16_t *r);
int16_t csubq_avx(int16_t *r);
int16_t tomont_avx(int16_t *r);
#endif

#endif
