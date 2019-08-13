#ifndef NTT_H
#define NTT_H

#include <stdint.h>

extern uint16_t PQCLEAN_NAMESPACE_zetas_exp[396];
extern uint16_t PQCLEAN_NAMESPACE_zetas_inv_exp[396];

void PQCLEAN_NAMESPACE_ntt_level0_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_NAMESPACE_ntt_levels1t6_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_NAMESPACE_invntt_levels0t5_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_NAMESPACE_invntt_level6_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_NAMESPACE_nttpack_avx(int16_t *r);
void PQCLEAN_NAMESPACE_nttunpack_avx(int16_t *r);
void PQCLEAN_NAMESPACE_basemul_avx(int16_t *r, const int16_t *a, const int16_t *b, uint16_t *zeta);
void PQCLEAN_NAMESPACE_basemul_acc_avx(int16_t *r, const int16_t *a, const int16_t *b, uint16_t *zeta);

void PQCLEAN_NAMESPACE_ntttobytes_avx(uint8_t *r, const int16_t *a);
void PQCLEAN_NAMESPACE_nttfrombytes_avx(int16_t *r, const uint8_t *a);

#endif
