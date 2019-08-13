#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

int16_t PQCLEAN_NAMESPACE_reduce_avx(int16_t *r);
int16_t PQCLEAN_NAMESPACE_csubq_avx(int16_t *r);
int16_t PQCLEAN_NAMESPACE_frommont_avx(int16_t *r);

#endif
