#ifndef NTT_H
#define NTT_H

#include "inttypes.h"

extern uint16_t omegas_montgomery[];
extern uint16_t omegas_inv_bitrev_montgomery[];

extern uint16_t psis_bitrev_montgomery[];
extern uint16_t psis_inv_montgomery[];

extern const uint16_t zetas_exp[];
extern const uint16_t zetas_inv_exp[];

void nttasm(uint16_t* rpoly, uint16_t* poly, const uint16_t* z) asm("nttasm");
void invnttasm(uint16_t* rpoly, uint16_t* poly, const uint16_t* z) asm("invnttasm");

void bitrev_vector(uint16_t* poly);
void mul_coefficients(uint16_t* poly, const uint16_t* factors);
void ntt(uint16_t* poly, const uint16_t* omegas);

#endif
