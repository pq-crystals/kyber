#ifndef POLYVEC_H
#define POLYVEC_H

#include "params.h"
#include "poly.h"

#include <stdint.h>

typedef struct{
  poly vec[KYBER_K];
} polyvec;

void polyvec_compress(uint8_t *r, polyvec *a);
void polyvec_decompress(polyvec *r, const uint8_t *a);

void polyvec_tobytes(uint8_t *r, polyvec *a);
void polyvec_frombytes(polyvec *r, const uint8_t *a);

void polyvec_ntt(polyvec *r);
void polyvec_invntt(polyvec *r);

void polyvec_pointwise_acc(poly *r, const polyvec *a, const polyvec *b);

void polyvec_reduce(polyvec *r);
void polyvec_csubq(polyvec *r);

void polyvec_add(polyvec *r, const polyvec *a, const polyvec *b);

#endif
