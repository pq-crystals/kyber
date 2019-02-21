#include "reduce.h"
#include "params.h"

/*************************************************
* Name:        montgomery_reduce
*
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q,
*              where R=2^18 (see value of rlog)
*
* Arguments:   - uint32_t a: input unsigned integer to be reduced; has to be in {0,...,XXX}
*
* Returns:     unsigned integer in {0,...,2^12-1} congruent to a * R^-1 modulo q.
**************************************************/
int16_t montgomery_reduce(int32_t a)
{
  int32_t t;
  int16_t u;

  u = a * QINV;
  t = (int32_t)u * KYBER_Q;
  t = a - t;
  t >>= 16;
  return t;
}


/*************************************************
* Name:        barrett_reduce
*
* Description: Barrett reduction; given a 16-bit integer a, computes
*              16-bit integer congruent to a mod q in {0,...,XXX}
*
* Arguments:   - int16_t a: input unsigned integer to be reduced
*
* Returns:     unsigned integer in {0,...,XXX} congruent to a modulo q.
* XXX: update comment, understand code!
**************************************************/
int16_t barrett_reduce(int16_t a)
{
  int16_t t;

  t = a & 0x7FF;
  a >>= 11;
  t -= (a << 10) + (a << 8) + a;
  return t;
}

/*************************************************
* Name:        freeze
*
* Description: Full reduction; given a 16-bit integer a, computes
*              unsigned integer a mod q.
*
* Arguments:   - int16_t x: input unsigned integer to be reduced
*
* Returns:     unsigned integer in {0,...,q-1} congruent to a modulo q.
**************************************************/
int16_t freeze(int16_t x) {
  //XXX: Fix
  while(x < 0)
    x += KYBER_Q;
  while(x >= KYBER_Q)
    x -= KYBER_Q;
  return x;
}
