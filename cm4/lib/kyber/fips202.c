#include "fips202.h"
#include <stdint.h>
#include <string.h>

/*************************************************
* Name:        keccak_absorb
* 
* Description: Absorb step of Keccak;
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - uint8_t *s:              pointer to (uninitialized) output Keccak state
*              - unsigned int r:          rate in bytes (e.g., 168 for SHAKE128)
*              - const unsigned char *m:  pointer to input to be absorbed into s
*              - unsigned long long mlen: length of input in bytes
*              - unsigned char p:         domain-separation byte for different Keccak-derived functions
**************************************************/
static void keccak_absorb(uint8_t* s, unsigned int r, const unsigned char *m,
                          unsigned long long int mlen, unsigned char p) {
  unsigned long long i;
  unsigned char t[200];

  // Zero state
  KeccakP1600_Initialize((unsigned char *)s);

  while (mlen >= (size_t)r) {
    KeccakP1600_AddBytes(s, m, 0, r);
    KeccakP1600_Permute_24rounds(s);
    m += r;
    mlen -= r;
  }

  for (i = 0; i < r; ++i) t[i] = 0;
  for (i = 0; i < mlen; ++i) t[i] = m[i];
  t[i] = p;
  t[r - 1] |= 128;
  KeccakP1600_AddBytes(s, t, 0, r);
}

/*************************************************
* Name:        keccak_squeezeblocks
* 
* Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
*              Modifies the state. Can be called multiple times to keep squeezing,
*              i.e., is incremental.
*
* Arguments:   - unsigned char *h:               pointer to output blocks
*              - unsigned long long int nblocks: number of blocks to be squeezed (written to h)
*              - uint8_t *s:                     pointer to in/output Keccak state
*              - unsigned int r:                 rate in bytes (e.g., 168 for SHAKE128)
**************************************************/
static void keccak_squeezeblocks(unsigned char *h,
                                 unsigned long long int nblocks, uint8_t* s,
                                 unsigned int r) {
  while (nblocks > 0) {
    KeccakP1600_Permute_24rounds(s);
    KeccakP1600_ExtractBytes(s, h, 0, r);
    h += r;
    nblocks--;
  }
}

/*************************************************
* Name:        shake128_absorb
* 
* Description: Absorb step of the SHAKE128 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - uint8_t *s:                      pointer to (uninitialized) output Keccak state
*              - const unsigned char *input:      pointer to input to be absorbed into s
*              - unsigned long long inputByteLen: length of input in bytes
**************************************************/
void shake128_absorb(uint8_t* s, const unsigned char *input,
                     unsigned int inputByteLen) {
  keccak_absorb(s, SHAKE128_RATE, input, inputByteLen, 0x1F);
}

/*************************************************
* Name:        shake128_squeezeblocks
* 
* Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of SHAKE128_RATE bytes each.
*              Modifies the state. Can be called multiple times to keep squeezing,
*              i.e., is incremental.
*
* Arguments:   - unsigned char *output:      pointer to output blocks
*              - unsigned long long nblocks: number of blocks to be squeezed (written to output)
*              - uint8_t *s:                 pointer to in/output Keccak state
**************************************************/
void shake128_squeezeblocks(unsigned char *output, unsigned long long nblocks,
                            uint8_t* s) {
  keccak_squeezeblocks(output, nblocks, s, SHAKE128_RATE);
}

/*************************************************
* Name:        shake256
* 
* Description: SHAKE256 XOF with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output
*              - unsigned long long outlen:  requested output length in bytes
*              - const unsigned char *input: pointer to input
*              - unsigned long long inlen:   length of input in bytes
**************************************************/
void shake256(unsigned char *output, unsigned long long outlen,
              const unsigned char *input, unsigned long long inlen) {
  uint8_t s[200];
  unsigned char t[SHAKE256_RATE];
  unsigned long long nblocks = outlen / SHAKE256_RATE;
  size_t i;

  /* Absorb input */
  keccak_absorb(s, SHAKE256_RATE, input, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks(output, nblocks, s, SHAKE256_RATE);

  output += nblocks * SHAKE256_RATE;
  outlen -= nblocks * SHAKE256_RATE;

  if (outlen) {
    keccak_squeezeblocks(t, 1, s, SHAKE256_RATE);
    for (i = 0; i < outlen; i++) output[i] = t[i];
  }
}

/*************************************************
* Name:        sha3_256
* 
* Description: SHA3-256 with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output
*              - const unsigned char *input: pointer to input 
*              - unsigned long long inlen:   length of input in bytes
**************************************************/
void sha3_256(unsigned char *output, const unsigned char *input,
              unsigned long long inlen) {
  uint8_t s[200];
  unsigned char t[SHA3_256_RATE];
  size_t i;

  /* Absorb input */
  keccak_absorb(s, SHA3_256_RATE, input, inlen, 0x06);

  /* Squeeze output */
  keccak_squeezeblocks(t, 1, s, SHA3_256_RATE);

  for (i = 0; i < 32; i++) output[i] = t[i];
}

/*************************************************
* Name:        sha3_512
* 
* Description: SHA3-512 with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output
*              - const unsigned char *input: pointer to input 
*              - unsigned long long inlen:   length of input in bytes
**************************************************/
void sha3_512(unsigned char *output, const unsigned char *input,
              unsigned long long inlen) {
  uint8_t s[200];
  unsigned char t[SHA3_512_RATE];
  size_t i;

  /* Absorb input */
  keccak_absorb(s, SHA3_512_RATE, input, inlen, 0x06);

  /* Squeeze output */
  keccak_squeezeblocks(t, 1, s, SHA3_512_RATE);

  for (i = 0; i < 64; i++) output[i] = t[i];
}
