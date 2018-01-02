#include "inttypes.h"
#include "ntt.h"
#include "params.h"
#include "reduce.h"

extern const uint16_t omegas_inv_bitrev_montgomery[];
extern const uint16_t psis_inv_montgomery[];
extern const uint16_t zetas[];

extern void ntt_asm(uint16_t* poly, const uint16_t* zetas);
extern void invntt_asm(uint16_t* poly, const uint16_t* omegas_inv_bitrev_montgomery);
extern void pointwise_multiply(uint16_t* poly, const uint16_t* psis_inv_montgomery);

/*************************************************
* Name:        ntt
* 
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial (vector of 256 coefficients) in place; 
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *p: pointer to in/output polynomial
**************************************************/
inline void ntt(uint16_t *p) 
{
  ntt_asm(p, zetas);
}

/*************************************************
* Name:        invntt
* 
* Description: Computes inverse of negacyclic number-theoretic transform (NTT) of
*              a polynomial (vector of 256 coefficients) in place; 
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
inline void invntt(uint16_t * a)
{
  invntt_asm(a, omegas_inv_bitrev_montgomery);
  pointwise_multiply(a, psis_inv_montgomery);
}
