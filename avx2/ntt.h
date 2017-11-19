#ifndef NTT_H
#define NTT_H

#include <stdint.h>

extern const uint16_t zetas_exp[];
extern const uint16_t zetas_inv_exp[];

void ntt(uint16_t *inout, const uint16_t* zetas);
void invntt(uint16_t *out, const uint16_t *in, const uint16_t* omegas);

#endif
