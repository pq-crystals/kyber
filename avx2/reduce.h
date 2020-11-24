#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"
#include <immintrin.h>

#define reduce_avx KYBER_NAMESPACE(_reduce_avx)
void reduce_avx(__m256i *r, const __m256i *qdata);
#define csubq_avx KYBER_NAMESPACE(_csubq_avx)
void csubq_avx(__m256i *r, const __m256i *qdata);
#define tomont_avx KYBER_NAMESPACE(_tomont_avx)
void tomont_avx(__m256i *r, const __m256i *qdata);

#endif
