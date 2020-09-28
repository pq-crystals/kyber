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
#if (KYBER_POLYCOMPRESSEDBYTES == 96)
void poly_compress(uint8_t r[96], const poly * restrict a)
{
  unsigned int i;
  __m256i f0, f1, f2, f3;
  __m128i t0, t1;
  const __m256i v = _mm256_load_si256((__m256i *)&qdata[_16XV]);
  const __m256i shift1 = _mm256_set1_epi16(1 << 8);
  const __m256i mask = _mm256_set1_epi16(7);
  const __m256i shift2 = _mm256_set1_epi16((8 << 8) + 1);
  const __m256i shift3 = _mm256_set1_epi32((64 << 16) + 1);
  const __m256i sllvdidx = _mm256_set1_epi64x(12LL << 32);
  const __m256i shufbidx = _mm256_set_epi8( 8, 2, 1, 0,-1,-1,-1,-1,14,13,12, 6, 5, 4,10, 9,
                                           -1,-1,-1,-1,14,13,12, 6, 5, 4,10, 9, 8, 2, 1, 0);

  for(i=0;i<KYBER_N/64;i++) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+ 0]);
    f1 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+16]);
    f2 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+32]);
    f3 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+48]);
    f0 = _mm256_mulhi_epi16(f0,v);
    f1 = _mm256_mulhi_epi16(f1,v);
    f2 = _mm256_mulhi_epi16(f2,v);
    f3 = _mm256_mulhi_epi16(f3,v);
    f0 = _mm256_mulhrs_epi16(f0,shift1);
    f1 = _mm256_mulhrs_epi16(f1,shift1);
    f2 = _mm256_mulhrs_epi16(f2,shift1);
    f3 = _mm256_mulhrs_epi16(f3,shift1);
    f0 = _mm256_and_si256(f0,mask);
    f1 = _mm256_and_si256(f1,mask);
    f2 = _mm256_and_si256(f2,mask);
    f3 = _mm256_and_si256(f3,mask);
    f0 = _mm256_packus_epi16(f0,f1);
    f2 = _mm256_packus_epi16(f2,f3);
    f0 = _mm256_maddubs_epi16(f0,shift2);	// a0 a1 a2 a3 b0 b1 b2 b3 a4 a5 a6 a7 b4 b5 b6 b7
    f2 = _mm256_maddubs_epi16(f2,shift2);	// c0 c1 c2 c3 d0 d1 d2 d3 c4 c5 c6 c7 d4 d5 d6 d7
    f0 = _mm256_madd_epi16(f0,shift3);		// a0 a1 b0 b1 a2 a3 b2 b3
    f2 = _mm256_madd_epi16(f2,shift3);		// c0 c1 d0 d1 c2 c3 d2 d3
    f0 = _mm256_sllv_epi32(f0,sllvdidx);
    f2 = _mm256_sllv_epi32(f2,sllvdidx);
    f0 = _mm256_hadd_epi32(f0,f2);		// a0 c0 c0 d0 a1 b1 c1 d1
    f0 = _mm256_permute4x64_epi64(f0,0xD8);	// a0 b0 a1 b1 c0 d0 c1 d1
    f0 = _mm256_shuffle_epi8(f0,shufbidx);
    t0 = _mm256_castsi256_si128(f0);
    t1 = _mm256_extracti128_si256(f0,1);
    t0 = _mm_blend_epi32(t0,t1,0x08);
    _mm_storeu_si128((__m128i *)&r[24*i+ 0],t0);
    _mm_storel_epi64((__m128i *)&r[24*i+16],t1);
  }
}

void poly_decompress(poly * restrict r, const uint8_t a[96+2])
{
  unsigned int i;
  __m256i f;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_16XQ]);
  const __m256i shufbidx = _mm256_set_epi8(5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,
                                           2,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0);
  const __m256i mask = _mm256_set_epi16(224,28,896,112,14,448,56,7,
                                        224,28,896,112,14,448,56,7);
  const __m256i shift = _mm256_set_epi16(128,1024,32,256,2048,64,512,4096,
                                         128,1024,32,256,2048,64,512,4096);

  for(i=0;i<KYBER_N/16;i++) {
    f = _mm256_castpd_si256(_mm256_broadcast_sd((double *)&a[6*i+ 0]));
    f = _mm256_shuffle_epi8(f,shufbidx);
    f = _mm256_and_si256(f,mask);
    f = _mm256_mullo_epi16(f,shift);
    f = _mm256_mulhrs_epi16(f,q);
    _mm256_store_si256((__m256i *)&r->coeffs[16*i],f);
  }
}

#elif (KYBER_POLYCOMPRESSEDBYTES == 128)

void poly_compress(uint8_t r[128], const poly * restrict a)
{
  unsigned int i;
  __m256i f0, f1, f2, f3;
  const __m256i v = _mm256_load_si256((__m256i *)&qdata[_16XV]);
  const __m256i shift1 = _mm256_set1_epi16(1 << 9);
  const __m256i mask = _mm256_set1_epi16(15);
  const __m256i shift2 = _mm256_set1_epi16((16 << 8) + 1);
  const __m256i permdidx = _mm256_set_epi32(7,3,6,2,5,1,4,0);

  for(i=0;i<KYBER_N/64;i++) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+ 0]);
    f1 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+16]);
    f2 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+32]);
    f3 = _mm256_load_si256((__m256i *)&a->coeffs[64*i+48]);
    f0 = _mm256_mulhi_epi16(f0,v);
    f1 = _mm256_mulhi_epi16(f1,v);
    f2 = _mm256_mulhi_epi16(f2,v);
    f3 = _mm256_mulhi_epi16(f3,v);
    f0 = _mm256_mulhrs_epi16(f0,shift1);
    f1 = _mm256_mulhrs_epi16(f1,shift1);
    f2 = _mm256_mulhrs_epi16(f2,shift1);
    f3 = _mm256_mulhrs_epi16(f3,shift1);
    f0 = _mm256_and_si256(f0,mask);
    f1 = _mm256_and_si256(f1,mask);
    f2 = _mm256_and_si256(f2,mask);
    f3 = _mm256_and_si256(f3,mask);
    f0 = _mm256_packus_epi16(f0,f1);
    f2 = _mm256_packus_epi16(f2,f3);
    f0 = _mm256_maddubs_epi16(f0,shift2);
    f2 = _mm256_maddubs_epi16(f2,shift2);
    f0 = _mm256_packus_epi16(f0,f2);
    f0 = _mm256_permutevar8x32_epi32(f0,permdidx);
    _mm256_storeu_si256((__m256i *)&r[32*i],f0);
  }
}

void poly_decompress(poly * restrict r, const uint8_t a[128])
{
  unsigned int i;
  __m256i f;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_16XQ]);
  const __m256i shufbidx = _mm256_set_epi8(7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,
                                           3,3,3,3,2,2,2,2,1,1,1,1,0,0,0,0);
  const __m256i mask = _mm256_set1_epi32(0x00F0000F);
  const __m256i shift = _mm256_set1_epi32((128 << 16) + 2048);

  for(i=0;i<KYBER_N/16;i++) {
    f = _mm256_broadcastq_epi64(_mm_loadl_epi64((__m128i *)&a[8*i]));
    f = _mm256_shuffle_epi8(f,shufbidx);
    f = _mm256_and_si256(f,mask);
    f = _mm256_mullo_epi16(f,shift);
    f = _mm256_mulhrs_epi16(f,q);
    _mm256_store_si256((__m256i *)&r->coeffs[16*i],f);
  }
}

#elif (KYBER_POLYCOMPRESSEDBYTES == 160)

void poly_compress(uint8_t r[160], const poly * restrict a)
{
  unsigned int i;
  __m256i f0, f1;
  __m128i t0, t1;
  const __m256i v = _mm256_load_si256((__m256i *)&qdata[_16XV]);
  const __m256i shift1 = _mm256_set1_epi16(1 << 10);
  const __m256i mask = _mm256_set1_epi16(31);
  const __m256i shift2 = _mm256_set1_epi16((32 << 8) + 1);
  const __m256i shift3 = _mm256_set1_epi32((1024 << 16) + 1);
  const __m256i sllvdidx = _mm256_set1_epi64x(12);
  const __m256i shufbidx = _mm256_set_epi8( 8,-1,-1,-1,-1,-1, 4, 3, 2, 1, 0,-1,12,11,10, 9,
                                           -1,12,11,10, 9, 8,-1,-1,-1,-1,-1 ,4, 3, 2, 1, 0);

  for(i=0;i<KYBER_N/32;i++) {
    f0 = _mm256_load_si256((__m256i *)&a->coeffs[32*i+ 0]);
    f1 = _mm256_load_si256((__m256i *)&a->coeffs[32*i+16]);
    f0 = _mm256_mulhi_epi16(f0,v);
    f1 = _mm256_mulhi_epi16(f1,v);
    f0 = _mm256_mulhrs_epi16(f0,shift1);
    f1 = _mm256_mulhrs_epi16(f1,shift1);
    f0 = _mm256_and_si256(f0,mask);
    f1 = _mm256_and_si256(f1,mask);
    f0 = _mm256_packus_epi16(f0,f1);
    f0 = _mm256_maddubs_epi16(f0,shift2);	// a0 a1 a2 a3 b0 b1 b2 b3 a4 a5 a6 a7 b4 b5 b6 b7
    f0 = _mm256_madd_epi16(f0,shift3);		// a0 a1 b0 b1 a2 a3 b2 b3
    f0 = _mm256_sllv_epi32(f0,sllvdidx);
    f0 = _mm256_srlv_epi64(f0,sllvdidx);
    f0 = _mm256_shuffle_epi8(f0,shufbidx);
    t0 = _mm256_castsi256_si128(f0);
    t1 = _mm256_extracti128_si256(f0,1);
    t0 = _mm_blendv_epi8(t0,t1,_mm256_castsi256_si128(shufbidx));
    _mm_storeu_si128((__m128i *)&r[20*i+ 0],t0);
    _mm_store_ss((float *)&r[20*i+16],_mm_castsi128_ps(t1));
  }
}

void poly_decompress(poly * restrict r, const uint8_t a[160+6])
{
  unsigned int i;
  __m256i f;
  const __m256i q = _mm256_load_si256((__m256i *)&qdata[_16XQ]);
  const __m256i shufbidx = _mm256_set_epi8(9,9,9,8,8,8,8,7,7,6,6,6,6,5,5,5,
                                           4,4,4,3,3,3,3,2,2,1,1,1,1,0,0,0);
  const __m256i mask = _mm256_set_epi16(248,1984,62,496,3968,124,992,31,
                                        248,1984,62,496,3968,124,992,31);
  const __m256i shift = _mm256_set_epi16(128,16,512,64,8,256,32,1024,
                                         128,16,512,64,8,256,32,1024);

  for(i=0;i<KYBER_N/16;i++) {
    f = _mm256_broadcastsi128_si256(_mm_loadu_si128((__m128i *)&a[10*i]));
    f = _mm256_shuffle_epi8(f,shufbidx);
    f = _mm256_and_si256(f,mask);
    f = _mm256_mullo_epi16(f,shift);
    f = _mm256_mulhrs_epi16(f,q);
    _mm256_store_si256((__m256i *)&r->coeffs[16*i],f);
  }
}

#endif

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
* Name:        poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r:             pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce:       one-byte input nonce
**************************************************/
void poly_getnoise_eta2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  __attribute__((aligned(32)))
  uint8_t buf[KYBER_ETA2*KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce);
  cbd_eta2(r, buf);
}


#ifndef KYBER_90S
void poly_getnoise_eta2_4x(poly *r0,
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
  uint8_t buf[4][160]; /* 160 instead of SHAKE256_RATE for better alignment */
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

  cbd_eta2(r0, buf[0]);
  cbd_eta2(r1, buf[1]);
  cbd_eta2(r2, buf[2]);
  cbd_eta2(r3, buf[3]);
}

#if KYBER_ETA1 == 3
void poly_getnoise_eta1_4x(poly *r0,
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
  uint8_t buf[4][288]; /* 288 instead of 2*SHAKE256_RATE for better alignment, also 2 extra bytes needed in cbd3 */
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
  shake256x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 2, &state);

  cbd_eta1(r0, buf[0]);
  cbd_eta1(r1, buf[1]);
  cbd_eta1(r2, buf[2]);
  cbd_eta1(r3, buf[3]);
}

void poly_getnoise_eta1122_4x(poly *r0,
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
  uint8_t buf[4][288]; /* 288 instead of 2*SHAKE256_RATE for better alignment, also 2 extra bytes needed in cbd3 */
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
  shake256x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 2, &state);

  cbd_eta1(r0, buf[0]);
  cbd_eta1(r1, buf[1]);
  cbd_eta2(r2, buf[2]);
  cbd_eta2(r3, buf[3]);
}
#endif
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
