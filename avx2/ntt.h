#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

#define ntt_avx KYBER_NAMESPACE(ntt_avx)
void ntt_avx(int16_t *r, const uint16_t *qdata);
#define invntt_avx KYBER_NAMESPACE(invntt_avx)
void invntt_avx(int16_t *r, const uint16_t *qdata);

#define nttpack_avx KYBER_NAMESPACE(nttpack_avx)
void nttpack_avx(int16_t *r, const uint16_t *qdata);
#define nttunpack_avx KYBER_NAMESPACE(nttunpack_avx)
void nttunpack_avx(int16_t *r, const uint16_t *qdata);

#define basemul_avx KYBER_NAMESPACE(basemul_avx)
void basemul_avx(int16_t *r,
                 const int16_t *a,
                 const int16_t *b,
                 const uint16_t *qdata);
#define basemul_acc_avx KYBER_NAMESPACE(basemul_acc_avx)
void basemul_acc_avx(int16_t *r,
                     const int16_t *a,
                     const int16_t *b,
                     const uint16_t *qdata);

#define ntttobytes_avx KYBER_NAMESPACE(ntttobytes_avx)
void ntttobytes_avx(uint8_t *r, const int16_t *a, const uint16_t *qdata);
#define nttfrombytes_avx KYBER_NAMESPACE(nttfrombytes_avx)
void nttfrombytes_avx(int16_t *r, const uint8_t *a, const uint16_t *qdata);

#endif
