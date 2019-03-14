#include <stdint.h>
#include <immintrin.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - unsigned char *r: pointer to output byte array
*              - const poly *a:    pointer to input polynomial
**************************************************/
void poly_compress(unsigned char * restrict r, const poly * restrict a)
{
  uint8_t t[8];
  int i,j,k=0;

#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 3) + KYBER_Q/2) / KYBER_Q) & 7;

    r[k]   =  t[0]       | (t[1] << 3) | (t[2] << 6);
    r[k+1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[k+2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    k += 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 4) + KYBER_Q/2) / KYBER_Q) & 15;

    r[k]   = t[0] | (t[1] << 4);
    r[k+1] = t[2] | (t[3] << 4);
    r[k+2] = t[4] | (t[5] << 4);
    r[k+3] = t[6] | (t[7] << 4);
    k += 4;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 5) + KYBER_Q/2) / KYBER_Q) & 31;

    r[k]   =  t[0]       | (t[1] << 5);
    r[k+1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
    r[k+2] = (t[3] >> 1) | (t[4] << 4);
    r[k+3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
    r[k+4] = (t[6] >> 2) | (t[7] << 3);
    k += 5;
  }
#else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {96, 128, 160}"
#endif
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r:                pointer to output polynomial
*              - const unsigned char *a: pointer to input byte array
**************************************************/
void poly_decompress(poly * restrict r, const unsigned char * restrict a)
{
  int i;
#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] =  (((a[0] & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+1] = ((((a[0] >> 3) & 7) * KYBER_Q)+ 4) >> 3;
    r->coeffs[i+2] = ((((a[0] >> 6) | ((a[1] << 2) & 4)) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+3] = ((((a[1] >> 1) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+4] = ((((a[1] >> 4) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+5] = ((((a[1] >> 7) | ((a[2] << 1) & 6)) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+6] = ((((a[2] >> 2) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+7] = ((((a[2] >> 5)) * KYBER_Q) + 4) >> 3;
    a += 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] = (((a[0] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+1] = (((a[0] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+2] = (((a[1] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+3] = (((a[1] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+4] = (((a[2] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+5] = (((a[2] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+6] = (((a[3] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+7] = (((a[3] >> 4) * KYBER_Q) + 8) >> 4;
    a += 4;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] =  (((a[0] & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+1] = ((((a[0] >> 5) | ((a[1] & 3) << 3)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+2] = ((((a[1] >> 2) & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+3] = ((((a[1] >> 7) | ((a[2] & 15) << 1)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+4] = ((((a[2] >> 4) | ((a[3] &  1) << 4)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+5] = ((((a[3] >> 1) & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+6] = ((((a[3] >> 6) | ((a[4] &  7) << 2)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+7] =  (((a[4] >> 3) * KYBER_Q) + 16) >> 5;
    a += 5;
  }
#else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {96, 128, 160}"
#endif
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - unsigned char *r: pointer to output byte array
*              - const poly *a:    pointer to input polynomial
**************************************************/
void poly_tobytes(unsigned char * restrict r, const poly * restrict a)
{
  int i;
  uint16_t t0, t1;

  for(i=0;i<KYBER_N/2;i++){
    t0 = a->coeffs[2*i];
    t1 = a->coeffs[2*i+1];
    r[3*i] = t0 & 0xff;
    r[3*i+1] = (t0 >> 8) | ((t1 & 0xf) << 4);
    r[3*i+2] = t1 >> 4;
  }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r:                pointer to output polynomial
*              - const unsigned char *a: pointer to input byte array
**************************************************/
void poly_frombytes(poly * restrict r, const unsigned char * restrict a)
{
  int i;

  for(i=0;i<KYBER_N/2;i++){
    r->coeffs[2*i]   = a[3*i]        | ((uint16_t)a[3*i+1] & 0x0f) << 8;
    r->coeffs[2*i+1] = a[3*i+1] >> 4 | ((uint16_t)a[3*i+2] & 0xff) << 4;
  }
}

/*************************************************
* Name:        poly_getnoise
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA
*
* Arguments:   - poly *r:                   pointer to output polynomial
*              - const unsigned char *seed: pointer to input seed
*              - unsigned char nonce:       one-byte input nonce
**************************************************/
void poly_getnoise(poly *r, const unsigned char *seed, unsigned char nonce)
{
  unsigned char buf[KYBER_ETA*KYBER_N/4];

  prf(buf, KYBER_ETA*KYBER_N/4, seed, nonce);
  cbd(r, buf);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void poly_ntt(poly *r)
{
  ntt_level0_avx(r->coeffs, zetas_exp);
  ntt_level0_avx(r->coeffs + 64, zetas_exp);
  ntt_levels1t6_avx(r->coeffs, zetas_exp + 4);
  ntt_levels1t6_avx(r->coeffs + 128, zetas_exp + 200);
}

/*************************************************
* Name:        poly_invntt
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt(poly *r)
{
  invntt_levels0t5_avx(r->coeffs, zetas_inv_exp);
  invntt_levels0t5_avx(r->coeffs + 128, zetas_inv_exp + 196);
  invntt_level6_avx(r->coeffs, zetas_inv_exp + 392);
}

// FIXME
void poly_nttpack(poly *r)
{
  nttpack_avx(r->coeffs);
  nttpack_avx(r->coeffs + 128);
}

// FIXME
void poly_nttunpack(poly *r)
{
  nttunpack_avx(r->coeffs);
  nttunpack_avx(r->coeffs + 128);
}

//XXX Add comment
void poly_basemul(poly *r, const poly *a, const poly *b)
{
  basemul_avx(r->coeffs,
              a->coeffs,
              b->coeffs,
              zetas_exp + 152);
  basemul_avx(r->coeffs + 64,
              a->coeffs + 64,
              b->coeffs + 64,
              zetas_exp + 184);
  basemul_avx(r->coeffs + 128,
              a->coeffs + 128,
              b->coeffs + 128,
              zetas_exp + 348);
  basemul_avx(r->coeffs + 192,
              a->coeffs + 192,
              b->coeffs + 192,
              zetas_exp + 380);
}

// FIXME
void poly_frommont(poly *r)
{
  frommont_avx(r->coeffs);
  frommont_avx(r->coeffs + 128);
}

// FIXME
void poly_reduce(poly *r)
{
  reduce_avx(r->coeffs);
  reduce_avx(r->coeffs + 128);
}

// FIXME
void poly_csubq(poly *r)
{
  csubq_avx(r->coeffs);
  csubq_avx(r->coeffs + 128);
}

/*************************************************
* Name:        poly_add
*
* Description: Add two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_add(poly *r, const poly *a, const poly *b)
{
  int i;
  __m256i vec0, vec1;

  for(i=0;i<KYBER_N;i+=16) {
    vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
    vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
    vec0 = _mm256_add_epi16(vec0, vec1);
    _mm256_store_si256((__m256i *)&r->coeffs[i], vec0);
  }
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_sub(poly *r, const poly *a, const poly *b)
{
  int i;
  __m256i vec0, vec1;

  for(i=0;i<KYBER_N;i+=16) {
    vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
    vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
    vec0 = _mm256_sub_epi16(vec0, vec1);
    _mm256_store_si256((__m256i *)&r->coeffs[i], vec0);
  }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r:                  pointer to output polynomial
*              - const unsigned char *msg: pointer to input message
**************************************************/
void poly_frommsg(poly * restrict r, const unsigned char msg[KYBER_SYMBYTES])
{
  int i,j;
  uint16_t mask;

  for(i=0;i<KYBER_SYMBYTES;i++)
  {
    for(j=0;j<8;j++)
    {
      mask = -((msg[i] >> j)&1);
      r->coeffs[8*i+j] = mask & ((KYBER_Q+1)/2);
    }
  }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - unsigned char *msg: pointer to output message
*              - const poly *a:      pointer to input polynomial
**************************************************/
void poly_tomsg(unsigned char msg[KYBER_SYMBYTES], const poly * restrict a)
{
  uint16_t t;
  int i,j;

  for(i=0;i<KYBER_SYMBYTES;i++)
  {
    msg[i] = 0;
    for(j=0;j<8;j++)
    {
      t = (((a->coeffs[8*i+j] << 1) + KYBER_Q/2) / KYBER_Q) & 1; // FIXME
      msg[i] |= t << j;
    }
  }
}
