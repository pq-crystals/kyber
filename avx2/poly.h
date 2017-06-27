#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

typedef struct{
  uint32_t coeffs[KYBER_N];
} poly;

void poly_compress(unsigned char *r, const poly *a);
void poly_decompress(poly *r, const unsigned char *a);

void poly_tobytes(unsigned char *r, const poly *a);
void poly_frombytes(poly *r, const unsigned char *a);

void poly_frommsg(poly *r, const unsigned char msg[KYBER_SHAREDKEYBYTES]);
void poly_tomsg(unsigned char msg[KYBER_SHAREDKEYBYTES], const poly *r);

void poly_getnoise(poly *r,const unsigned char *seed, unsigned char nonce);
void poly_getnoise_local(poly *r,const unsigned char *seed, unsigned char nonce);

void poly_getnoise4x(poly *r0, poly *r1, poly *r2, poly *r3, 
                     const unsigned char *seed, 
                     unsigned char nonce0, 
                     unsigned char nonce1, 
                     unsigned char nonce2, 
                     unsigned char nonce3);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
  
void poly_add(poly *r, const poly *a, const poly *b);
void poly_sub(poly *r, const poly *a, const poly *b);

#endif
