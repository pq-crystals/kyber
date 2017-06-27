#ifndef NTT_H
#define NTT_H

#include "inttypes.h"

extern double omegas_bitrev[];
extern double omegas_inv_bitrev[];

extern uint32_t psis_bitrev[];
extern uint32_t psis_inv[];

void bitrev_vector(uint32_t* poly);
void mul_coefficients(uint32_t* poly, const uint32_t* factors);
void ntt(uint32_t* poly, const double* omegas);

#endif
