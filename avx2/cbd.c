#include <stdint.h>
#include <immintrin.h>
#include "params.h"
#include "cbd.h"

/*************************************************
* Name:        load24_littleendian
*
* Description: load 3 bytes into a 32-bit integer
*              in little-endian order
*              This function is only needed for Kyber-512
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns 32-bit unsigned integer loaded from x (most significant byte is zero)
**************************************************/
#if KYBER_ETA1 == 3
static uint32_t load24_littleendian(const uint8_t x[3])
{
  uint32_t r;
  r  = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  return r;
}
#endif


/*************************************************
* Name:        cbd2
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=2
*
* Arguments:   - poly *r:                  pointer to output polynomial
*              - const unsigned char *buf: pointer to input byte array
**************************************************/
static void cbd2(poly * restrict r, const uint8_t * restrict buf)
{
  unsigned int i;
  __m256i vec0, vec1, vec2, vec3, tmp;
  const __m256i mask55 = _mm256_set1_epi32(0x55555555);
  const __m256i mask33 = _mm256_set1_epi32(0x33333333);
  const __m256i mask03 = _mm256_set1_epi32(0x03030303);

  for(i = 0; i < KYBER_N/64; i++) {
    vec0 = _mm256_load_si256((__m256i *)&buf[32*i]);

    vec1 = _mm256_srli_epi32(vec0, 1);
    vec0 = _mm256_and_si256(mask55, vec0);
    vec1 = _mm256_and_si256(mask55, vec1);
    vec0 = _mm256_add_epi32(vec0, vec1);

    vec1 = _mm256_srli_epi32(vec0, 2);
    vec0 = _mm256_and_si256(mask33, vec0);
    vec1 = _mm256_and_si256(mask33, vec1);

    vec2 = _mm256_srli_epi32(vec0, 4);
    vec3 = _mm256_srli_epi32(vec1, 4);
    vec0 = _mm256_and_si256(mask03, vec0);
    vec1 = _mm256_and_si256(mask03, vec1);
    vec2 = _mm256_and_si256(mask03, vec2);
    vec3 = _mm256_and_si256(mask03, vec3);

    vec1 = _mm256_sub_epi8(vec0, vec1);
    vec3 = _mm256_sub_epi8(vec2, vec3);

    vec0 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(vec1));
    vec1 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(vec1,1));
    vec2 = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(vec3));
    vec3 = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(vec3,1));

    tmp = _mm256_unpacklo_epi16(vec0, vec2);
    vec2 = _mm256_unpackhi_epi16(vec0, vec2);
    vec0 = tmp;
    tmp = _mm256_unpacklo_epi16(vec1, vec3);
    vec3 = _mm256_unpackhi_epi16(vec1, vec3);
    vec1 = tmp;

    tmp = _mm256_permute2x128_si256(vec0, vec2, 0x20);
    vec2 = _mm256_permute2x128_si256(vec0, vec2, 0x31);
    vec0 = tmp;
    tmp = _mm256_permute2x128_si256(vec1, vec3, 0x20);
    vec3 = _mm256_permute2x128_si256(vec1, vec3, 0x31);
    vec1 = tmp;

    _mm256_store_si256((__m256i *)&r->coeffs[64*i+0], vec0);
    _mm256_store_si256((__m256i *)&r->coeffs[64*i+16], vec2);
    _mm256_store_si256((__m256i *)&r->coeffs[64*i+32], vec1);
    _mm256_store_si256((__m256i *)&r->coeffs[64*i+48], vec3);
  }
}

/*************************************************
* Name:        cbd3
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=3
*              This function is only needed for Kyber-512
*
* Arguments:   - poly *r:            pointer to output polynomial
*              - const uint8_t *buf: pointer to input byte array
**************************************************/
#if KYBER_ETA1 == 3
static void cbd3(poly *r, const uint8_t buf[3*KYBER_N/4])
{
  unsigned int i,j;
  uint32_t t,d;
  int16_t a,b;

  for(i=0;i<KYBER_N/4;i++) {
    t  = load24_littleendian(buf+3*i);
    d  = t & 0x00249249;
    d += (t>>1) & 0x00249249;
    d += (t>>2) & 0x00249249;

    for(j=0;j<4;j++) {
      a = (d >> (6*j+0)) & 0x7;
      b = (d >> (6*j+3)) & 0x7;
      r->coeffs[4*i+j] = a - b;
    }
  }
}
#endif

void cbd_eta1(poly *r, const uint8_t buf[KYBER_ETA1*KYBER_N/4])
{
#if KYBER_ETA1 == 2
  cbd2(r, buf);
#elif KYBER_ETA1 == 3
  cbd3(r, buf);
#else
#error "This implementation requires eta1 in {2,3}"
#endif
}

void cbd_eta2(poly *r, const uint8_t buf[KYBER_ETA1*KYBER_N/4])
{
#if KYBER_ETA2 != 2
#error "This implementation requires eta2 = 2"
#else
  cbd2(r, buf);
#endif
}
