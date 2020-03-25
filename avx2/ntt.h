#ifndef NTT_H
#define NTT_H

#include "params.h"
#include "consts.h"

#define ntt_level0_avx KYBER_NAMESPACE(ntt_level0_avx)
#define ntt_levels1t6_avx KYBER_NAMESPACE(ntt_levels1t6_avx)
#define invntt_levels0t5_avx KYBER_NAMESPACE(invntt_levels0t5_avx)
#define invntt_level6_avx KYBER_NAMESPACE(invntt_level6_avx)
#define nttpack_avx KYBER_NAMESPACE(nttpack_avx)
#define nttunpack_avx KYBER_NAMESPACE(nttunpack_avx)
#define basemul_avx KYBER_NAMESPACE(basemul_avx)
#define basemul_acc_avx KYBER_NAMESPACE(basemul_acc_avx)
#define ntttobytes_avx KYBER_NAMESPACE(ntttobytes_avx)
#define nttfrombytes_avx KYBER_NAMESPACE(nttfrombytes_avx)

#ifndef __ASSEMBLER__
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

#endif
