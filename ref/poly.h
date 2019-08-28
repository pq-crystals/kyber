#ifndef POLY_H
#define POLY_H

#include "params.h"

#include <stdint.h>
/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct{
  int16_t coeffs[KYBER_N];
} poly;

void poly_compress(uint8_t *r, poly *a);
void poly_decompress(poly *r, const uint8_t *a);

void poly_tobytes(uint8_t *r, poly *a);
void poly_frombytes(poly *r, const uint8_t *a);

void poly_frommsg(poly *r, const uint8_t msg[KYBER_SYMBYTES]);
void poly_tomsg(uint8_t msg[KYBER_SYMBYTES], poly *a);

void poly_getnoise(poly *r,const uint8_t *seed, uint8_t nonce);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_basemul(poly *r, const poly *a, const poly *b);
void poly_frommont(poly *r);

void poly_reduce(poly *r);
void poly_csubq(poly *r);

void poly_add(poly *r, const poly *a, const poly *b);
void poly_sub(poly *r, const poly *a, const poly *b);

#endif
