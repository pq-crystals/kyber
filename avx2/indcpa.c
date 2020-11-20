#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "indcpa.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "ntt.h"
#include "symmetric.h"
#include "rejsample.h"
#include "cbd.h"

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   uint8_t *r:          pointer to the output serialized public key
*              polyvec *pk:         pointer to the input public-key polyvec
*              const uint8_t *seed: pointer to the input public seed
**************************************************/
static void pack_pk(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES],
                    polyvec *pk,
                    const uint8_t seed[KYBER_SYMBYTES])
{
  size_t i;
  polyvec_tobytes(r, pk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    r[i+KYBER_POLYVECBYTES] = seed[i];
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
*              - uint8_t *seed: pointer to output seed to generate matrix A
*              - const uint8_t *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk,
                      uint8_t seed[KYBER_SYMBYTES],
                      const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES])
{
  size_t i;
  polyvec_frombytes(pk, packedpk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    seed[i] = packedpk[i+KYBER_POLYVECBYTES];
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8_t *r:  pointer to output serialized secret key
*              - polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES], polyvec *sk)
{
  polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key;
*              inverse of pack_sk
*
* Arguments:   - polyvec *sk: pointer to output vector of polynomials
*                (secret key)
*              - const uint8_t *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk,
                      const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES])
{
  polyvec_frombytes(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8_t *r: pointer to the output serialized ciphertext
*              poly *pk:   pointer to the input vector of polynomials b
*              poly *v:    pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(uint8_t r[KYBER_INDCPA_BYTES],
                            polyvec *b,
                            poly *v)
{
  polyvec_compress(r, b);
  poly_compress(r+KYBER_POLYVECCOMPRESSEDBYTES, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b:       pointer to the output vector of polynomials b
*              - poly *v:          pointer to the output polynomial v
*              - const uint8_t *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b,
                              poly *v,
                              const uint8_t c[KYBER_INDCPA_BYTES+6])
{
  polyvec_decompress(b, c);
  poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r: pointer to output buffer
*              - unsigned int len: requested number of 16-bit integers
*                (uniform mod q)
*              - const uint8_t *buf: pointer to input buffer
*                (assumed to be uniform random bytes)
*              - unsigned int buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint16_t val0, val1;

  ctr = pos = 0;
  while(ctr < len && pos + 3 <= buflen) {
    val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
    val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4));
    pos += 3;

    if(val0 < KYBER_Q)
      r[ctr++] = val0;
    if(ctr < len && val1 < KYBER_Q)
      r[ctr++] = val1;
  }

  return ctr;
}

#define gen_a(A,B)  gen_matrix(A,B,0)
#define gen_at(A,B) gen_matrix(A,B,1)

/*************************************************
* Name:        gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a: pointer to ouptput matrix A
*              - const uint8_t *seed: pointer to input seed
*              - int transposed: boolean deciding whether A or A^T is generated
**************************************************/
#define GEN_MATRIX_NBLOCKS (AVX_REJ_UNIFORM_BUFLEN/XOF_BLOCKBYTES)

#ifdef KYBER_90S
void gen_matrix(polyvec *a, const uint8_t seed[KYBER_SYMBYTES], int transposed)
{
  unsigned int ctr, i, j, k;
  unsigned buflen, off;
  __attribute__((aligned(16)))
  uint64_t nonce;
  __attribute__((aligned(32)))
  uint8_t buf[AVX_REJ_UNIFORM_BUFLEN+2];
  aes256ctr_ctx state;

  aes256ctr_init(&state, seed, 0);

  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_K;j++) {
      if(transposed)
        nonce = (j << 8) | i;
      else
        nonce = (i << 8) | j;

      state.n = _mm_loadl_epi64((__m128i *)&nonce);
      aes256ctr_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform_avx(a[i].vec[j].coeffs, buf);

      while(ctr < KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        aes256ctr_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, buflen);
      }

      poly_nttunpack(&a[i].vec[j]);
    }
  }
}
#else
#if KYBER_K == 2
void gen_matrix(polyvec *a, const uint8_t seed[32], int transposed)
{
  unsigned int ctr0, ctr1, ctr2, ctr3;
  __attribute__((aligned(32)))
  uint8_t buf[4][AVX_REJ_UNIFORM_BUFLEN];
  __m256i f;
  keccakx4_state state;

  f = _mm256_load_si256((__m256i *)seed);
  _mm256_store_si256((__m256i *)buf[0], f);
  _mm256_store_si256((__m256i *)buf[1], f);
  _mm256_store_si256((__m256i *)buf[2], f);
  _mm256_store_si256((__m256i *)buf[3], f);

  if(transposed) {
    buf[0][KYBER_SYMBYTES+0] = 0;
    buf[0][KYBER_SYMBYTES+1] = 0;
    buf[1][KYBER_SYMBYTES+0] = 0;
    buf[1][KYBER_SYMBYTES+1] = 1;
    buf[2][KYBER_SYMBYTES+0] = 1;
    buf[2][KYBER_SYMBYTES+1] = 0;
    buf[3][KYBER_SYMBYTES+0] = 1;
    buf[3][KYBER_SYMBYTES+1] = 1;
  }
  else {
    buf[0][KYBER_SYMBYTES+0] = 0;
    buf[0][KYBER_SYMBYTES+1] = 0;
    buf[1][KYBER_SYMBYTES+0] = 1;
    buf[1][KYBER_SYMBYTES+1] = 0;
    buf[2][KYBER_SYMBYTES+0] = 0;
    buf[2][KYBER_SYMBYTES+1] = 1;
    buf[3][KYBER_SYMBYTES+0] = 1;
    buf[3][KYBER_SYMBYTES+1] = 1;
  }

  shake128x4_absorb(&state, buf[0], buf[1], buf[2], buf[3], KYBER_SYMBYTES+2);
  shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], GEN_MATRIX_NBLOCKS,
                           &state);

  ctr0 = rej_uniform_avx(a[0].vec[0].coeffs, buf[0]);
  ctr1 = rej_uniform_avx(a[0].vec[1].coeffs, buf[1]);
  ctr2 = rej_uniform_avx(a[1].vec[0].coeffs, buf[2]);
  ctr3 = rej_uniform_avx(a[1].vec[1].coeffs, buf[3]);

  while(ctr0 < KYBER_N || ctr1 < KYBER_N || ctr2 < KYBER_N || ctr3 < KYBER_N) {
    shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 1, &state);

    ctr0 += rej_uniform(a[0].vec[0].coeffs + ctr0, KYBER_N - ctr0, buf[0],
                        XOF_BLOCKBYTES);
    ctr1 += rej_uniform(a[0].vec[1].coeffs + ctr1, KYBER_N - ctr1, buf[1],
                        XOF_BLOCKBYTES);
    ctr2 += rej_uniform(a[1].vec[0].coeffs + ctr2, KYBER_N - ctr2, buf[2],
                        XOF_BLOCKBYTES);
    ctr3 += rej_uniform(a[1].vec[1].coeffs + ctr3, KYBER_N - ctr3, buf[3],
                        XOF_BLOCKBYTES);
  }

  poly_nttunpack(&a[0].vec[0]);
  poly_nttunpack(&a[0].vec[1]);
  poly_nttunpack(&a[1].vec[0]);
  poly_nttunpack(&a[1].vec[1]);
}
#elif KYBER_K == 3
void gen_matrix(polyvec *a, const uint8_t seed[32], int transposed)
{
  unsigned int ctr0, ctr1, ctr2, ctr3;
  __attribute__((aligned(32)))
  uint8_t buf[4][(GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+31)/32*32];
  __m256i f;
  keccakx4_state state;
  keccak_state state1x;

  f = _mm256_load_si256((__m256i *)seed);
  _mm256_store_si256((__m256i *)buf[0], f);
  _mm256_store_si256((__m256i *)buf[1], f);
  _mm256_store_si256((__m256i *)buf[2], f);
  _mm256_store_si256((__m256i *)buf[3], f);

  if(transposed) {
    buf[0][KYBER_SYMBYTES+0] = 0;
    buf[0][KYBER_SYMBYTES+1] = 0;
    buf[1][KYBER_SYMBYTES+0] = 0;
    buf[1][KYBER_SYMBYTES+1] = 1;
    buf[2][KYBER_SYMBYTES+0] = 0;
    buf[2][KYBER_SYMBYTES+1] = 2;
    buf[3][KYBER_SYMBYTES+0] = 1;
    buf[3][KYBER_SYMBYTES+1] = 0;
  }
  else {
    buf[0][KYBER_SYMBYTES+0] = 0;
    buf[0][KYBER_SYMBYTES+1] = 0;
    buf[1][KYBER_SYMBYTES+0] = 1;
    buf[1][KYBER_SYMBYTES+1] = 0;
    buf[2][KYBER_SYMBYTES+0] = 2;
    buf[2][KYBER_SYMBYTES+1] = 0;
    buf[3][KYBER_SYMBYTES+0] = 0;
    buf[3][KYBER_SYMBYTES+1] = 1;
  }

  shake128x4_absorb(&state, buf[0], buf[1], buf[2], buf[3], KYBER_SYMBYTES+2);
  shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], GEN_MATRIX_NBLOCKS,
                           &state);

  ctr0 = rej_uniform_avx(a[0].vec[0].coeffs, buf[0]);
  ctr1 = rej_uniform_avx(a[0].vec[1].coeffs, buf[1]);
  ctr2 = rej_uniform_avx(a[0].vec[2].coeffs, buf[2]);
  ctr3 = rej_uniform_avx(a[1].vec[0].coeffs, buf[3]);

  while(ctr0 < KYBER_N || ctr1 < KYBER_N || ctr2 < KYBER_N || ctr3 < KYBER_N) {
    shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 1, &state);

    ctr0 += rej_uniform(a[0].vec[0].coeffs + ctr0, KYBER_N - ctr0, buf[0],
                        XOF_BLOCKBYTES);
    ctr1 += rej_uniform(a[0].vec[1].coeffs + ctr1, KYBER_N - ctr1, buf[1],
                        XOF_BLOCKBYTES);
    ctr2 += rej_uniform(a[0].vec[2].coeffs + ctr2, KYBER_N - ctr2, buf[2],
                        XOF_BLOCKBYTES);
    ctr3 += rej_uniform(a[1].vec[0].coeffs + ctr3, KYBER_N - ctr3, buf[3],
                        XOF_BLOCKBYTES);
  }

  poly_nttunpack(&a[0].vec[0]);
  poly_nttunpack(&a[0].vec[1]);
  poly_nttunpack(&a[0].vec[2]);
  poly_nttunpack(&a[1].vec[0]);

  f = _mm256_load_si256((__m256i *)seed);
  _mm256_store_si256((__m256i *)buf[0], f);
  _mm256_store_si256((__m256i *)buf[1], f);
  _mm256_store_si256((__m256i *)buf[2], f);
  _mm256_store_si256((__m256i *)buf[3], f);

  if(transposed) {
    buf[0][KYBER_SYMBYTES+0] = 1;
    buf[0][KYBER_SYMBYTES+1] = 1;
    buf[1][KYBER_SYMBYTES+0] = 1;
    buf[1][KYBER_SYMBYTES+1] = 2;
    buf[2][KYBER_SYMBYTES+0] = 2;
    buf[2][KYBER_SYMBYTES+1] = 0;
    buf[3][KYBER_SYMBYTES+0] = 2;
    buf[3][KYBER_SYMBYTES+1] = 1;
  }
  else {
    buf[0][KYBER_SYMBYTES+0] = 1;
    buf[0][KYBER_SYMBYTES+1] = 1;
    buf[1][KYBER_SYMBYTES+0] = 2;
    buf[1][KYBER_SYMBYTES+1] = 1;
    buf[2][KYBER_SYMBYTES+0] = 0;
    buf[2][KYBER_SYMBYTES+1] = 2;
    buf[3][KYBER_SYMBYTES+0] = 1;
    buf[3][KYBER_SYMBYTES+1] = 2;
  }

  shake128x4_absorb(&state, buf[0], buf[1], buf[2], buf[3], KYBER_SYMBYTES+2);
  shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], GEN_MATRIX_NBLOCKS,
                           &state);

  ctr0 = rej_uniform_avx(a[1].vec[1].coeffs, buf[0]);
  ctr1 = rej_uniform_avx(a[1].vec[2].coeffs, buf[1]);
  ctr2 = rej_uniform_avx(a[2].vec[0].coeffs, buf[2]);
  ctr3 = rej_uniform_avx(a[2].vec[1].coeffs, buf[3]);

  while(ctr0 < KYBER_N || ctr1 < KYBER_N || ctr2 < KYBER_N || ctr3 < KYBER_N) {
    shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 1, &state);

    ctr0 += rej_uniform(a[1].vec[1].coeffs + ctr0, KYBER_N - ctr0, buf[0],
                        XOF_BLOCKBYTES);
    ctr1 += rej_uniform(a[1].vec[2].coeffs + ctr1, KYBER_N - ctr1, buf[1],
                        XOF_BLOCKBYTES);
    ctr2 += rej_uniform(a[2].vec[0].coeffs + ctr2, KYBER_N - ctr2, buf[2],
                        XOF_BLOCKBYTES);
    ctr3 += rej_uniform(a[2].vec[1].coeffs + ctr3, KYBER_N - ctr3, buf[3],
                        XOF_BLOCKBYTES);
  }

  poly_nttunpack(&a[1].vec[1]);
  poly_nttunpack(&a[1].vec[2]);
  poly_nttunpack(&a[2].vec[0]);
  poly_nttunpack(&a[2].vec[1]);

  f = _mm256_load_si256((__m256i *)seed);
  _mm256_store_si256((__m256i *)buf[0], f);
  buf[0][KYBER_SYMBYTES+0] = 2;
  buf[0][KYBER_SYMBYTES+1] = 2;
  shake128_absorb_once(&state1x, buf[0], KYBER_SYMBYTES+2);
  shake128_squeezeblocks(buf[0], GEN_MATRIX_NBLOCKS, &state1x);
  ctr0 = rej_uniform_avx(a[2].vec[2].coeffs, buf[0]);
  while(ctr0 < KYBER_N)
  {
    shake128_squeezeblocks(buf[0], 1, &state1x);
    ctr0 += rej_uniform(a[2].vec[2].coeffs + ctr0, KYBER_N - ctr0, buf[0],
                        XOF_BLOCKBYTES);
  }

  poly_nttunpack(&a[2].vec[2]);
}
#elif KYBER_K == 4
void gen_matrix(polyvec *a, const uint8_t seed[32], int transposed)
{
  unsigned int i, ctr0, ctr1, ctr2, ctr3;
  __attribute__((aligned(32)))
  uint8_t buf[4][(GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+31)/32*32];
  __m256i f;
  keccakx4_state state;

  for(i=0;i<4;i++) {
    f = _mm256_load_si256((__m256i *)seed);
    _mm256_store_si256((__m256i *)buf[0], f);
    _mm256_store_si256((__m256i *)buf[1], f);
    _mm256_store_si256((__m256i *)buf[2], f);
    _mm256_store_si256((__m256i *)buf[3], f);

    if(transposed) {
      buf[0][KYBER_SYMBYTES+0] = i;
      buf[0][KYBER_SYMBYTES+1] = 0;
      buf[1][KYBER_SYMBYTES+0] = i;
      buf[1][KYBER_SYMBYTES+1] = 1;
      buf[2][KYBER_SYMBYTES+0] = i;
      buf[2][KYBER_SYMBYTES+1] = 2;
      buf[3][KYBER_SYMBYTES+0] = i;
      buf[3][KYBER_SYMBYTES+1] = 3;
    }
    else {
      buf[0][KYBER_SYMBYTES+0] = 0;
      buf[0][KYBER_SYMBYTES+1] = i;
      buf[1][KYBER_SYMBYTES+0] = 1;
      buf[1][KYBER_SYMBYTES+1] = i;
      buf[2][KYBER_SYMBYTES+0] = 2;
      buf[2][KYBER_SYMBYTES+1] = i;
      buf[3][KYBER_SYMBYTES+0] = 3;
      buf[3][KYBER_SYMBYTES+1] = i;
    }

    shake128x4_absorb(&state, buf[0], buf[1], buf[2], buf[3], KYBER_SYMBYTES+2);
    shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3],
                             GEN_MATRIX_NBLOCKS, &state);

    ctr0 = rej_uniform_avx(a[i].vec[0].coeffs, buf[0]);
    ctr1 = rej_uniform_avx(a[i].vec[1].coeffs, buf[1]);
    ctr2 = rej_uniform_avx(a[i].vec[2].coeffs, buf[2]);
    ctr3 = rej_uniform_avx(a[i].vec[3].coeffs, buf[3]);

    while(ctr0 < KYBER_N || ctr1 < KYBER_N || ctr2 < KYBER_N || ctr3 < KYBER_N) {
      shake128x4_squeezeblocks(buf[0], buf[1], buf[2], buf[3], 1, &state);

      ctr0 += rej_uniform(a[i].vec[0].coeffs + ctr0, KYBER_N - ctr0, buf[0],
                          XOF_BLOCKBYTES);
      ctr1 += rej_uniform(a[i].vec[1].coeffs + ctr1, KYBER_N - ctr1, buf[1],
                          XOF_BLOCKBYTES);
      ctr2 += rej_uniform(a[i].vec[2].coeffs + ctr2, KYBER_N - ctr2, buf[2],
                          XOF_BLOCKBYTES);
      ctr3 += rej_uniform(a[i].vec[3].coeffs + ctr3, KYBER_N - ctr3, buf[3],
                          XOF_BLOCKBYTES);
    }

    poly_nttunpack(&a[i].vec[0]);
    poly_nttunpack(&a[i].vec[1]);
    poly_nttunpack(&a[i].vec[2]);
    poly_nttunpack(&a[i].vec[3]);
  }
}
#endif
#endif

/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
                              (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void indcpa_keypair(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                    uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES])
{
  unsigned int i;
  __attribute__((aligned(32)))
  uint8_t buf[2*KYBER_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+KYBER_SYMBYTES;
  polyvec a[KYBER_K], e, pkpv, skpv;

  randombytes(buf, KYBER_SYMBYTES);
  hash_g(buf, buf, KYBER_SYMBYTES);

  gen_a(a, publicseed);

#ifdef KYBER_90S
#define NBLOCKS ((2*KYBER_ETA1*32)/AES256CTR_BLOCKBYTES ) /* Assumes divisibility */
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  aes256ctr_ctx state;
  __attribute__((aligned(32)))
  uint8_t coins[AES256CTR_BLOCKBYTES*NBLOCKS+2]; /* +2 as required by cbd3 */
  aes256ctr_init(&state, noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++) {
    aes256ctr_squeezeblocks(coins, NBLOCKS, &state);
    state.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
    cbd_eta1(&skpv.vec[i], coins);
  }
  for(i=0;i<KYBER_K;i++) {
    aes256ctr_squeezeblocks(coins, NBLOCKS, &state);
    state.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
    cbd_eta1(&e.vec[i], coins);
  }
#else
#if KYBER_K == 2
  poly_getnoise_eta1_4x(skpv.vec+0, skpv.vec+1, e.vec+0, e.vec+1, noiseseed,
      0, 1, 2, 3);
#elif KYBER_K == 3
#if KYBER_ETA1 == KYBER_ETA2
  poly_getnoise_eta2_4x(skpv.vec+0, skpv.vec+1, skpv.vec+2, e.vec+0, noiseseed,
                  0, 1, 2, 3);
  poly_getnoise_eta2_4x(e.vec+1, e.vec+2, pkpv.vec+0, pkpv.vec+1, noiseseed,
                  4, 5, 6, 7);
#else
#error "We need eta1 == eta2 here"
#endif
#elif KYBER_K == 4
#if KYBER_ETA1 == KYBER_ETA2
  poly_getnoise_eta2_4x(skpv.vec+0, skpv.vec+1, skpv.vec+2, skpv.vec+3, noiseseed,
                  0, 1, 2, 3);
  poly_getnoise_eta2_4x(e.vec+0, e.vec+1, e.vec+2, e.vec+3, noiseseed,
                  4, 5, 6, 7);
#else
#error "We need eta1 == eta2 here"
#endif
#endif
#endif

  polyvec_ntt(&skpv);
  polyvec_reduce(&skpv);
  polyvec_ntt(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
    polyvec_pointwise_acc_montgomery(&pkpv.vec[i], &a[i], &skpv);
    poly_tomont(&pkpv.vec[i]);
  }

  polyvec_add(&pkpv, &pkpv, &e);
  polyvec_reduce(&pkpv);

  pack_sk(sk, &skpv);
  pack_pk(pk, &pkpv, publicseed);
}

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *c:           pointer to output ciphertext
*                                      (of length KYBER_INDCPA_BYTES bytes)
*              - const uint8_t *m:     pointer to input message
*                                      (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const uint8_t *pk:    pointer to input public key
*                                      (of length KYBER_INDCPA_PUBLICKEYBYTES)
*              - const uint8_t *coins: pointer to input random coins
*                                      used as seed (of length KYBER_SYMBYTES)
*                                      to deterministically generate all
*                                      randomness
**************************************************/
void indcpa_enc(uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  __attribute__((aligned(32)))
  uint8_t seed[KYBER_SYMBYTES];
  polyvec sp, pkpv, ep, at[KYBER_K], bp;
  poly v, k, epp;

  unpack_pk(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at(at, seed);

#ifdef KYBER_90S
#define NBLOCKS ((2*KYBER_ETA1*32)/AES256CTR_BLOCKBYTES ) /* Assumes divisibility */
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  aes256ctr_ctx state;
  __attribute__((aligned(32)))
  uint8_t buf[AES256CTR_BLOCKBYTES*NBLOCKS+2]; /* +2 as required by cbd3 */
  aes256ctr_init(&state, coins, nonce++);
  for(i=0;i<KYBER_K;i++) {
    aes256ctr_squeezeblocks(buf, NBLOCKS, &state);
    state.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
    cbd_eta1(&sp.vec[i], buf);
  }
  for(i=0;i<KYBER_K;i++) {
    aes256ctr_squeezeblocks(buf, 2, &state);
    state.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
    cbd_eta2(&ep.vec[i], buf);
  }
  aes256ctr_squeezeblocks(buf, 2, &state);
  state.n = _mm_loadl_epi64((__m128i *)&nonce);
  nonce++;
  cbd_eta2(&epp, buf);
#else
#if KYBER_K == 2
  poly_getnoise_eta1122_4x(sp.vec+0, sp.vec+1, ep.vec+0, ep.vec+1, coins, 0, 1, 2, 3);
  poly_getnoise_eta2(&epp, coins, 4);
#elif KYBER_K == 3
#if KYBER_ETA1 == KYBER_ETA2
  poly_getnoise_eta2_4x(sp.vec+0, sp.vec+1, sp.vec+2, ep.vec+0, coins,
                  0, 1, 2 ,3);
  poly_getnoise_eta2_4x(ep.vec+1, ep.vec+2, &epp, bp.vec+0, coins,
                  4, 5, 6, 7);
#else
#error "We need eta1 == eta2 here"
#endif
#elif KYBER_K == 4
#if KYBER_ETA1 == KYBER_ETA2
  poly_getnoise_eta2_4x(sp.vec+0, sp.vec+1, sp.vec+2, sp.vec+3, coins,
                  0, 1, 2, 3);
  poly_getnoise_eta2_4x(ep.vec+0, ep.vec+1, ep.vec+2, ep.vec+3, coins,
                  4, 5, 6, 7);
  poly_getnoise_eta2(&epp, coins, 8);
#else
#error "We need eta1 == eta2 here"
#endif
#endif
#endif

  polyvec_ntt(&sp);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_pointwise_acc_montgomery(&bp.vec[i], &at[i], &sp);
  polyvec_pointwise_acc_montgomery(&v, &pkpv, &sp);

  polyvec_invntt_tomont(&bp);
  poly_invntt_tomont(&v);

  polyvec_add(&bp, &bp, &ep);
  poly_add(&v, &v, &epp);
  poly_add(&v, &v, &k);
  polyvec_reduce(&bp);
  poly_reduce(&v);

  pack_ciphertext(c, &bp, &v);
}

/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *m:        pointer to output decrypted message
*                                   (of length KYBER_INDCPA_MSGBYTES)
*              - const uint8_t *c:  pointer to input ciphertext
*                                   (of length KYBER_INDCPA_BYTES)
*              - const uint8_t *sk: pointer to input secret key
*                                   (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void indcpa_dec(uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES])
{
  polyvec bp, skpv;
  poly v, mp;

  unpack_ciphertext(&bp, &v, c);
  unpack_sk(&skpv, sk);

  polyvec_ntt(&bp);
  polyvec_pointwise_acc_montgomery(&mp, &skpv, &bp);
  poly_invntt_tomont(&mp);

  poly_sub(&mp, &v, &mp);
  poly_reduce(&mp);

  poly_tomsg(m, &mp);
}
