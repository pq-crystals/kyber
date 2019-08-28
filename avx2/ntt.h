#ifndef NTT_H
#define NTT_H

#include "consts.h"

#include <stdint.h>

void ntt_level0_avx(int16_t *r, const uint16_t *zetas);
void ntt_levels1t6_avx(int16_t *r, const uint16_t *zetas);
void invntt_levels0t5_avx(int16_t *r, const uint16_t *zetas);
void invntt_level6_avx(int16_t *r, const uint16_t *zetas);
void nttpack_avx(int16_t *r);
void nttunpack_avx(int16_t *r);
void basemul_avx(int16_t *r, const int16_t *a, const int16_t *b, const uint16_t *zeta);
void basemul_acc_avx(int16_t *r, const int16_t *a, const int16_t *b, const uint16_t *zeta);

void ntttobytes_avx(uint8_t *r, const int16_t *a);
void nttfrombytes_avx(int16_t *r, const uint8_t *a);

#endif
