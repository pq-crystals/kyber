#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
#include "randombytes.h"

#define NTESTS 1000

static int test_keys()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
  unsigned char key_a[CRYPTO_BYTES];
  unsigned char key_b[CRYPTO_BYTES];

  for(i=0;i<NTESTS;i++) {
    //Alice generates a public key
    crypto_kem_keypair(pk, sk);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(ct, key_b, pk);

    //Alice uses Bobs response to get her shared key
    crypto_kem_dec(key_a, ct, sk);

    if(memcmp(key_a, key_b, CRYPTO_BYTES))
      printf("ERROR keys\n");
  }

  return 0;
}

static int test_invalid_sk_a()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
  unsigned char key_a[CRYPTO_BYTES];
  unsigned char key_b[CRYPTO_BYTES];

  for(i=0;i<NTESTS;i++) {
    //Alice generates a public key
    crypto_kem_keypair(pk, sk);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(ct, key_b, pk);

    //Replace secret key with random values
    randombytes(sk, CRYPTO_SECRETKEYBYTES);

    //Alice uses Bobs response to get her shared key
    crypto_kem_dec(key_a, ct, sk);

    if(!memcmp(key_a, key_b, CRYPTO_BYTES))
      printf("ERROR invalid sk\n");
  }

  return 0;
}

static int test_invalid_ciphertext()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
  unsigned char key_a[CRYPTO_BYTES];
  unsigned char key_b[CRYPTO_BYTES];
  uint8_t b;
  size_t pos;

  for(i=0;i<NTESTS;i++) {
    do {
      randombytes(&b, sizeof(uint8_t));
    } while(!b);
    randombytes((uint8_t *)&pos, sizeof(size_t));

    //Alice generates a public key
    crypto_kem_keypair(pk, sk);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(ct, key_b, pk);

    //Change some byte in the ciphertext (i.e., encapsulated key)
    ct[pos % CRYPTO_CIPHERTEXTBYTES] ^= b;

    //Alice uses Bobs response to get her shared key
    crypto_kem_dec(key_a, ct, sk);

    if(!memcmp(key_a, key_b, CRYPTO_BYTES))
      printf("ERROR invalid ciphertext\n");
  }

  return 0;
}

int main(void)
{
  test_keys();
  test_invalid_sk_a();
  test_invalid_ciphertext();

  printf("CRYPTO_SECRETKEYBYTES:  %d\n",CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_PUBLICKEYBYTES:  %d\n",CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_CIPHERTEXTBYTES: %d\n",CRYPTO_CIPHERTEXTBYTES);

  return 0;
}
