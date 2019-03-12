#include <stdint.h>
#include "indcpa.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "ntt.h"
#include "symmetric.h"
#include "rejsample.h"

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              compressed and serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   unsigned char *r:          pointer to the output serialized public key
*              const poly *pk:            pointer to the input public-key polynomial
*              const unsigned char *seed: pointer to the input public seed
**************************************************/
static void pack_pk(unsigned char *r, polyvec *pk, const unsigned char *seed)
{
  int i;
  polyvec_csubq(pk);
  polyvec_nttpack(pk);
  polyvec_tobytes(r, pk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    r[i+KYBER_POLYVECBYTES] = seed[i];
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize and decompress public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk:                   pointer to output public-key vector of polynomials
*              - unsigned char *seed:           pointer to output seed to generate matrix A
*              - const unsigned char *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk, unsigned char *seed, const unsigned char *packedpk)
{
  int i;
  polyvec_frombytes(pk, packedpk);
  polyvec_nttunpack(pk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    seed[i] = packedpk[i+KYBER_POLYVECBYTES];
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   unsigned char *r:          pointer to the output serialized ciphertext
*              const poly *pk:            pointer to the input vector of polynomials b
*              const unsigned char *seed: pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(unsigned char *r, polyvec *b, poly *v)
{
  polyvec_csubq(b);
  poly_csubq(v);
  polyvec_compress(r, b);
  poly_compress(r+KYBER_POLYVECCOMPRESSEDBYTES, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b:             pointer to the output vector of polynomials b
*              - poly *v:                pointer to the output polynomial v
*              - const unsigned char *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b, poly *v, const unsigned char *c)
{
  polyvec_decompress(b, c);
  poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - unsigned char *r:  pointer to output serialized secret key
*              - const polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(unsigned char *r, polyvec *sk)
{
  polyvec_csubq(sk);
  //polyvec_nttpack(sk);
  polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key;
*              inverse of pack_sk
*
* Arguments:   - polyvec *sk:                   pointer to output vector of polynomials (secret key)
*              - const unsigned char *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk, const unsigned char *packedsk)
{
  polyvec_frombytes(sk, packedsk);
  //polyvec_nttunpack(sk);
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
* Arguments:   - polyvec *a:                pointer to ouptput matrix A
*              - const unsigned char *seed: pointer to input seed
*              - int transposed:            boolean deciding whether A or A^T is generated
**************************************************/
void gen_matrix(polyvec *a, const unsigned char *seed, int transposed) // Not static for benchmarking
{
  unsigned int ctr, offset, bufbytes, i, j, k;
  //const unsigned int maxnblocks=(473+XOF_BLOCKBYTES)/XOF_BLOCKBYTES; /* 473 is expected number of required bytes */
  const unsigned int maxnblocks=(630+XOF_BLOCKBYTES)/XOF_BLOCKBYTES; /* 473 is expected number of required bytes */
  uint8_t buf[XOF_BLOCKBYTES*maxnblocks+2];
  xof_state state;

  for(i=0;i<KYBER_K;i++)
  {
    for(j=0;j<KYBER_K;j++)
    {
      if(transposed) {
        xof_absorb(&state, seed, i, j);
      }
      else {
        xof_absorb(&state, seed, j, i);
      }

      xof_squeezeblocks(buf, maxnblocks, &state);
      bufbytes = maxnblocks*XOF_BLOCKBYTES;

      ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, bufbytes);

      while(ctr < KYBER_N)
      {
        offset = bufbytes % 2;
        for(k=0;k<offset;k++)
          buf[k] = buf[bufbytes-offset+k];

        xof_squeezeblocks(buf+offset, 1, &state);
        bufbytes = XOF_BLOCKBYTES+offset;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, bufbytes);
      }

      poly_reduce(&a[i].vec[j]);
    }
  }
}

/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - unsigned char *pk: pointer to output public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void indcpa_keypair(unsigned char *pk, unsigned char *sk)
{
  polyvec a[KYBER_K], e, pkpv, skpv;
  unsigned char buf[2*KYBER_SYMBYTES];
  unsigned char *publicseed = buf;
  unsigned char *noiseseed = buf+KYBER_SYMBYTES;
  int i;
  unsigned char nonce=0;

  randombytes(buf, KYBER_SYMBYTES);
  hash_g(buf, buf, KYBER_SYMBYTES);

  gen_a(a, publicseed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise(skpv.vec+i, noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise(e.vec+i, noiseseed, nonce++);

  polyvec_ntt(&skpv);
  polyvec_ntt(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
    polyvec_pointwise_acc(&pkpv.vec[i], &a[i], &skpv);
    poly_frommont(&pkpv.vec[i]);
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
* Arguments:   - unsigned char *c:          pointer to output ciphertext (of length KYBER_INDCPA_BYTES bytes)
*              - const unsigned char *m:    pointer to input message (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const unsigned char *pk:   pointer to input public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - const unsigned char *coin: pointer to input random coins used as seed (of length KYBER_SYMBYTES bytes)
*                                           to deterministically generate all randomness
**************************************************/
void indcpa_enc(unsigned char *c,
                const unsigned char *m,
                const unsigned char *pk,
                const unsigned char *coins)
{
  polyvec sp, pkpv, ep, at[KYBER_K], bp;
  poly v, k, epp;
  unsigned char seed[KYBER_SYMBYTES];
  int i;
  unsigned char nonce=0;

  unpack_pk(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at(at, seed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise(sp.vec+i, coins, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise(ep.vec+i, coins, nonce++);
  poly_getnoise(&epp, coins, nonce++);

  polyvec_ntt(&sp);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_pointwise_acc(&bp.vec[i], &at[i], &sp);

  polyvec_pointwise_acc(&v, &pkpv, &sp);

  polyvec_invntt(&bp);
  poly_invntt(&v);

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
* Arguments:   - unsigned char *m:        pointer to output decrypted message (of length KYBER_INDCPA_MSGBYTES)
*              - const unsigned char *c:  pointer to input ciphertext (of length KYBER_INDCPA_BYTES)
*              - const unsigned char *sk: pointer to input secret key (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void indcpa_dec(unsigned char *m,
                const unsigned char *c,
                const unsigned char *sk)
{
  polyvec bp, skpv;
  poly v, mp;

  unpack_ciphertext(&bp, &v, c);
  unpack_sk(&skpv, sk);

  polyvec_ntt(&bp);
  polyvec_pointwise_acc(&mp, &skpv, &bp);
  poly_invntt(&mp);

  poly_sub(&mp, &v, &mp);
  poly_reduce(&mp);

  poly_csubq(&mp);
  poly_tomsg(m, &mp);
}
