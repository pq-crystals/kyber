#ifndef POLY_H
#define POLY_H

#include "params.h"

#include <immintrin.h>
#include <stdint.h>

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef union{
  int16_t coeffs[KYBER_N];
  __m256i _dummy;
} poly;

void poly_compress(uint8_t *r, poly *a);
void poly_decompress(poly *r, const uint8_t *a);

void poly_tobytes(uint8_t *r, poly *a);
void poly_frombytes(poly *r, const uint8_t *a);

void poly_frommsg(poly *r, const uint8_t msg[KYBER_SYMBYTES]);
void poly_tomsg(uint8_t msg[KYBER_SYMBYTES], poly *a);

void poly_getnoise(poly *r, const uint8_t *seed, uint8_t nonce);
#ifndef KYBER_90S
void poly_getnoise4x(poly *r0,
                     poly *r1,
                     poly *r2,
                     poly *r3,
                     const uint8_t *seed,
                     uint8_t nonce0,
                     uint8_t nonce1,
                     uint8_t nonce2,
                     uint8_t nonce3);
#endif


void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_nttunpack(poly *r);
void poly_basemul(poly *r, const poly *a, const poly *b);
void poly_frommont(poly *r);

void poly_reduce(poly *r);
void poly_csubq(poly *r);

void poly_add(poly *r, const poly *a, const poly *b);
void poly_sub(poly *r, const poly *a, const poly *b);

#endif
