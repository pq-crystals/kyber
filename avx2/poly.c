#include <stdint.h>
#include <immintrin.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "consts.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - poly *a:    pointer to input polynomial
**************************************************/
void poly_compress(uint8_t r[KYBER_POLYCOMPRESSEDBYTES], poly * restrict a)
{
  unsigned int i,j;
  uint8_t t[8];

  poly_csubq(a);

#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++)
      t[j] = ((((uint16_t)a->coeffs[8*i+j] << 3) + KYBER_Q/2)/KYBER_Q) & 7;

    r[0] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    r += 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++)
      t[j] = ((((uint16_t)a->coeffs[8*i+j] << 4) + KYBER_Q/2)/KYBER_Q) & 15;

    r[0] = t[0] | (t[1] << 4);
    r[1] = t[2] | (t[3] << 4);
    r[2] = t[4] | (t[5] << 4);
    r[3] = t[6] | (t[7] << 4);
    r += 4;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[8*i+j] << 5) + KYBER_Q/2)/KYBER_Q) & 31;

    r[0] = (t[0] >> 0) | (t[1] << 5);
    r[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
    r[2] = (t[3] >> 1) | (t[4] << 4);
    r[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
    r[4] = (t[6] >> 2) | (t[7] << 3);
    r += 5;
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
* Arguments:   - poly *r:          pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress(poly * restrict r,
                     const uint8_t a[KYBER_POLYCOMPRESSEDBYTES])
{
  unsigned int i;

#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  unsigned int j;
  uint8_t t[8];
  for(i=0;i<KYBER_N/8;i++) {
    t[0] = (a[0] >> 0);
    t[1] = (a[0] >> 3);
    t[2] = (a[0] >> 6) | (a[1] << 2);
    t[3] = (a[1] >> 1);
    t[4] = (a[1] >> 4);
    t[5] = (a[1] >> 7) | (a[2] << 1);
    t[6] = (a[2] >> 2);
    t[7] = (a[2] >> 5);
    a += 3;

    for(j=0;j<8;j++)
      r->coeffs[8*i+j] = ((uint16_t)(t[j] & 7)*KYBER_Q + 4) >> 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N/2;i++) {
    r->coeffs[2*i+0] = (((uint16_t)(a[0] & 15)*KYBER_Q) + 8) >> 4;
    r->coeffs[2*i+1] = (((uint16_t)(a[0] >> 4)*KYBER_Q) + 8) >> 4;
    a += 1;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  unsigned int j;
  uint8_t t[8];
  for(i=0;i<KYBER_N/8;i++) {
    t[0] = (a[0] >> 0);
    t[1] = (a[0] >> 5) | (a[1] << 3);
    t[2] = (a[1] >> 2);
    t[3] = (a[1] >> 7) | (a[2] << 1);
    t[4] = (a[2] >> 4) | (a[3] << 4);
    t[5] = (a[3] >> 1);
    t[6] = (a[3] >> 6) | (a[4] << 2);
    t[7] = (a[4] >> 3);
    a += 5;

    for(j=0;j<8;j++)
      r->coeffs[8*i+j] = ((uint32_t)(t[j] & 31)*KYBER_Q + 16) >> 5;
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
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - poly *a:    pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t r[KYBER_POLYBYTES], poly *a)
{
  ntttobytes_avx(r, a->coeffs, qdata);
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r:          pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES])
{
  nttfrombytes_avx(r->coeffs, a, qdata);
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r:            pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void poly_frommsg(poly * restrict r,
                  const uint8_t msg[KYBER_INDCPA_MSGBYTES])
{
#if (KYBER_INDCPA_MSGBYTES != 32)
#error "KYBER_INDCPA_MSGBYTES must be equal to 32!"
#endif
  __m256i f, g0, g1, g2, g3, h0, h1, h2, h3;
  const __m256i shift = _mm256_broadcastsi128_si256(_mm_set_epi32(0,1,2,3));
  const __m256i idx = _mm256_broadcastsi128_si256(_mm_set_epi8(15,14,11,10,7,6,3,2,13,12,9,8,5,4,1,0));
  const __m256i hqs = _mm256_set1_epi16((KYBER_Q+1)/2);

#define FROMMSG64(i)						\
  g3 = _mm256_shuffle_epi32(f,0x55*i);				\
  g3 = _mm256_sllv_epi32(g3,shift);				\
  g3 = _mm256_shuffle_epi8(g3,idx);				\
  g0 = _mm256_slli_epi16(g3,12);				\
  g1 = _mm256_slli_epi16(g3,8);					\
  g2 = _mm256_slli_epi16(g3,4);					\
  g0 = _mm256_srai_epi16(g0,15);				\
  g1 = _mm256_srai_epi16(g1,15);				\
  g2 = _mm256_srai_epi16(g2,15);				\
  g3 = _mm256_srai_epi16(g3,15);				\
  g0 = _mm256_and_si256(g0,hqs);  /* 19 18 17 16  3  2  1  0 */	\
  g1 = _mm256_and_si256(g1,hqs);  /* 23 22 21 20  7  6  5  4 */	\
  g2 = _mm256_and_si256(g2,hqs);  /* 27 26 25 24 11 10  9  8 */	\
  g3 = _mm256_and_si256(g3,hqs);  /* 31 30 29 28 15 14 13 12 */	\
  h0 = _mm256_unpacklo_epi64(g0,g1);				\
  h2 = _mm256_unpackhi_epi64(g0,g1);				\
  h1 = _mm256_unpacklo_epi64(g2,g3);				\
  h3 = _mm256_unpackhi_epi64(g2,g3);				\
  g0 = _mm256_permute2x128_si256(h0,h1,0x20);			\
  g2 = _mm256_permute2x128_si256(h0,h1,0x31);			\
  g1 = _mm256_permute2x128_si256(h2,h3,0x20);			\
  g3 = _mm256_permute2x128_si256(h2,h3,0x31);			\
  _mm256_store_si256((__m256i *)&r->coeffs[  0+32*i+ 0],g0);	\
  _mm256_store_si256((__m256i *)&r->coeffs[  0+32*i+16],g1);	\
  _mm256_store_si256((__m256i *)&r->coeffs[128+32*i+ 0],g2);	\
  _mm256_store_si256((__m256i *)&r->coeffs[128+32*i+16],g3)

  f = _mm256_load_si256((__m256i *)msg);
  FROMMSG64(0);
  FROMMSG64(1);
  FROMMSG64(2);
  FROMMSG64(3);
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - poly *a:      pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], poly * restrict a)
{
  unsigned int i;
  uint32_t small;
  __m256i f0, f1, g0, g1;
  const __m256i hqs = _mm256_set1_epi16((KYBER_Q - 1)/2);
  const __m256i hhqs = _mm256_set1_epi16((KYBER_Q - 5)/4);

  for(i=0;i<KYBER_N/32;i++) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[32*i]);
    f1 = _mm256_load_si256((__m256i *)&a->coeffs[32*i+16]);
    f0 = _mm256_sub_epi16(hqs, f0);
    f1 = _mm256_sub_epi16(hqs, f1);
    g0 = _mm256_srai_epi16(f0, 15);
    g1 = _mm256_srai_epi16(f1, 15);
    f0 = _mm256_xor_si256(f0, g0);
    f1 = _mm256_xor_si256(f1, g1);
    f0 = _mm256_sub_epi16(hhqs, f0);
    f1 = _mm256_sub_epi16(hhqs, f1);
    f0 = _mm256_packs_epi16(f0, f1);
    small = _mm256_movemask_epi8(f0);
    small = ~small;
    msg[4*i+0] = small;
    msg[4*i+1] = small >> 16;
    msg[4*i+2] = small >>  8;
    msg[4*i+3] = small >> 24;
  }
}

/*************************************************
* Name:        poly_getnoise
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA
*
* Arguments:   - poly *r:             pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce:       one-byte input nonce
**************************************************/
void poly_getnoise(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  __attribute__((aligned(32)))
  uint8_t buf[KYBER_ETA*KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce);
  cbd(r, buf);
}

#ifndef KYBER_90S
void poly_getnoise4x(poly *r0,
                     poly *r1,
                     poly *r2,
                     poly *r3,
                     const uint8_t seed[32],
                     uint8_t nonce0,
                     uint8_t nonce1,
                     uint8_t nonce2,
                     uint8_t nonce3)
{
  __attribute__((aligned(32)))
  uint8_t buf[4][160];
  __m256i f;
  keccakx4_state state;

  f = _mm256_load_si256((__m256i *)seed);
  _mm256_store_si256((__m256i *)buf[0], f);
  _mm256_store_si256((__m256i *)buf[1], f);
  _mm256_store_si256((__m256i *)buf[2], f);
  _mm256_store_si256((__m256i *)buf[3], f);

  buf[0][32] = nonce0;
  buf[1][32] = nonce1;
  buf[2][32] = nonce2;
  buf[3][32] = nonce3;

  shake256x4_absorb(&state, buf[0], buf[1], buf[2], buf[3], 33);
  shake256x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 1, &state);

  cbd(r0, buf[0]);
  cbd(r1, buf[1]);
  cbd(r2, buf[2]);
  cbd(r3, buf[3]);
}
#endif

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
  ntt_avx(r->coeffs, qdata);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt_tomont(poly *r)
{
  invntt_avx(r->coeffs, qdata);
}

void poly_nttunpack(poly *r)
{
  nttunpack_avx(r->coeffs, qdata);
}

/*************************************************
* Name:        poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_basemul_montgomery(poly *r, const poly *a, const poly *b)
{
  basemul_avx(r->coeffs, a->coeffs, b->coeffs, qdata);
}

/*************************************************
* Name:        poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_tomont(poly *r)
{
  tomont_avx(r->coeffs, qdata);
}

/*************************************************
* Name:        poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *r)
{
  reduce_avx(r->coeffs, qdata);
}

/*************************************************
* Name:        poly_csubq
*
* Description: Applies conditional subtraction of q to each coefficient
*              of a polynomial. For details of conditional subtraction
*              of q see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_csubq(poly *r)
{
  csubq_avx(r->coeffs, qdata);
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
  unsigned int i;
  __m256i f0, f1;

  for(i=0;i<KYBER_N;i+=16) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
    f1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
    f0 = _mm256_add_epi16(f0, f1);
    _mm256_store_si256((__m256i *)&r->coeffs[i], f0);
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
  unsigned int i;
  __m256i f0, f1;

  for(i=0;i<KYBER_N;i+=16) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
    f1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
    f0 = _mm256_sub_epi16(f0, f1);
    _mm256_store_si256((__m256i *)&r->coeffs[i], f0);
  }
}
