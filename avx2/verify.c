#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include "verify.h"

/*************************************************
* Name:        verify
*
* Description: Compare two arrays for equality in constant time.
*
* Arguments:   const unsigned char *a: pointer to first byte array
*              const unsigned char *b: pointer to second byte array
*              size_t len:             length of the byte arrays
*
* Returns 0 if the byte arrays are equal, 1 otherwise
**************************************************/
int verify(const uint8_t *a, const uint8_t *b, size_t len)
{
  size_t pos;
  uint64_t r;
  __m256i avec, bvec, cvec;

  cvec = _mm256_setzero_si256();
  for(pos = 0; pos + 32 <= len; pos += 32) {
    avec = _mm256_loadu_si256((__m256i *)&a[pos]);
    bvec = _mm256_loadu_si256((__m256i *)&b[pos]);
    avec = _mm256_xor_si256(avec, bvec);
    cvec = _mm256_or_si256(cvec, avec);
  }
  r = 1 - _mm256_testz_si256(cvec,cvec);

  if(pos < len) {
    avec = _mm256_loadu_si256((__m256i *)&a[pos]);
    bvec = _mm256_loadu_si256((__m256i *)&b[pos]);
    cvec = _mm256_cmpeq_epi8(avec, bvec);
    r |= _mm256_movemask_epi8(cvec) & (-(uint32_t)1 >> (32 + pos - len));
  }

  r = (-r) >> 63;
  return r;
}

/*************************************************
* Name:        cmov
*
* Description: Copy len bytes from x to r if b is 1;
*              don't modify x if b is 0. Requires b to be in {0,1};
*              assumes two's complement representation of negative integers.
*              Runs in constant time.
*
* Arguments:   unsigned char *r:       pointer to output byte array
*              const unsigned char *x: pointer to input byte array
*              size_t len:             Amount of bytes to be copied
*              unsigned char b:        Condition bit; has to be in {0,1}
**************************************************/
void cmov(uint8_t * restrict r, const uint8_t * restrict x, size_t len, uint8_t b)
{
  size_t pos;
  __m256i xvec, rvec, bvec;

  b = -b;
  bvec = _mm256_set1_epi8(b);

  for(pos = 0; pos + 32 <= len; pos += 32) {
    rvec = _mm256_loadu_si256((__m256i *)&r[pos]);
    xvec = _mm256_loadu_si256((__m256i *)&x[pos]);
    xvec = _mm256_xor_si256(xvec, rvec);
    xvec = _mm256_and_si256(xvec, bvec);
    rvec = _mm256_xor_si256(rvec, xvec);
    _mm256_storeu_si256((__m256i *)&r[pos], rvec);
  }

  while(pos < len) {
    r[pos] ^= b & (x[pos] ^ r[pos]);
    pos += 1;
  }
}
