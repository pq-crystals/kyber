#include "kyber.h"
#include "randombytes.h"
#include "fips202.h"
#include "params.h"
#include "verify.h"
#include "indcpa.h"

// API FUNCTIONS 

/* Build a CCA-secure KEM from an IND-CPA-secure encryption scheme */

void crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
  size_t i;
  indcpa_keypair(pk, sk);
  for(i=0;i<KYBER_INDCPA_PUBLICKEYBYTES;i++)
    sk[i+KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
  shake128(sk+KYBER_SECRETKEYBYTES-64,32,pk,KYBER_PUBLICKEYBYTES);
  randombytes(sk+KYBER_SECRETKEYBYTES-KYBER_SHAREDKEYBYTES,KYBER_SHAREDKEYBYTES);        /* Value z for pseudo-random output on reject */
}

void crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk)
{
  unsigned char krq[96];                             /* Will contain key, coins, qrom-hash */
  unsigned char buf[64];                          
  int i;

  randombytes(buf, 32);
  shake128(buf,32,buf,32);                           /* Don't release system RNG output */

  shake128(buf+32, 32, pk, KYBER_PUBLICKEYBYTES);    /* Multitarget countermeasure for coins + contributory KEM */
  shake128(krq, 96, buf, 64);

  indcpa_enc(c, buf, pk, krq+32);                    /* coins are in krq+32 */

  for(i=0;i<32;i++)
    c[i+KYBER_INDCPA_BYTES] = krq[i+64];

  shake128(krq+32, 32, c, KYBER_BYTES);              /* overwrite coins in krq with h(c) */
  shake128(k, 32, krq, 64);                          /* hash concatenation of pre-k and h(c) to k */
}

void crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk)
{
  int i, fail;
  unsigned char cmp[KYBER_BYTES];
  unsigned char buf[64];
  unsigned char krq[96];                             /* Will contain key, coins, qrom-hash */
  const unsigned char *pk = sk+KYBER_INDCPA_SECRETKEYBYTES;

  indcpa_dec(buf, c, sk);

  // shake128(buf+32, 32, pk, KYBER_PUBLICKEYBYTES); /* Multitarget countermeasure for coins + contributory KEM */
  for(i=0;i<32;i++)                                  /* Save hash by storing h(pk) in sk */
    buf[32+i] = sk[KYBER_SECRETKEYBYTES-64+i];
  shake128(krq, 96, buf, 64);

  indcpa_enc(cmp, buf, pk, krq+32);                  /* coins are in krq+32 */

  for(i=0;i<32;i++)
    cmp[i+KYBER_INDCPA_BYTES] = krq[i+64];

  fail = verify(c, cmp, KYBER_BYTES);

  shake128(krq+32, 32, c, KYBER_BYTES);              /* overwrite coins in krq with h(c)  */

  cmov(krq, sk+KYBER_SECRETKEYBYTES-KYBER_SHAREDKEYBYTES, KYBER_SHAREDKEYBYTES, fail); /* Overwrite pre-k with z on re-encryption failure */

  shake128(k, 32, krq, 64);                          /* hash concatenation of pre-k and h(c) to k */
}
