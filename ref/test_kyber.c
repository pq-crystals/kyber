#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 10

#define CRYPTO_BYTES PQCLEAN_NAMESPACE_CRYPTO_BYTES
#define CRYPTO_PUBLICKEYBYTES PQCLEAN_NAMESPACE_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES PQCLEAN_NAMESPACE_CRYPTO_CIPHERTEXTBYTES
#define CRYPTO_SECRETKEYBYTES PQCLEAN_NAMESPACE_CRYPTO_SECRETKEYBYTES

int test_keys()
{
  unsigned char key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sendb[CRYPTO_CIPHERTEXTBYTES];
  unsigned char sk_a[CRYPTO_SECRETKEYBYTES];
  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    PQCLEAN_NAMESPACE_crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    PQCLEAN_NAMESPACE_crypto_kem_enc(sendb, key_b, pk);

    //Alice uses Bobs response to get her secret key
    PQCLEAN_NAMESPACE_crypto_kem_dec(key_a, sendb, sk_a);

    if(memcmp(key_a, key_b, CRYPTO_BYTES))
      printf("ERROR keys\n");
  }

  return 0;
}


int test_invalid_sk_a()
{
  unsigned char sk_a[CRYPTO_SECRETKEYBYTES];
  unsigned char key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sendb[CRYPTO_CIPHERTEXTBYTES];
  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    PQCLEAN_NAMESPACE_crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    PQCLEAN_NAMESPACE_crypto_kem_enc(sendb, key_b, pk);

    //Replace secret key with random values
    randombytes(sk_a, CRYPTO_SECRETKEYBYTES);


    //Alice uses Bobs response to get her secre key
    PQCLEAN_NAMESPACE_crypto_kem_dec(key_a, sendb, sk_a);

    if(!memcmp(key_a, key_b, CRYPTO_BYTES))
      printf("ERROR invalid sk_a\n");
  }

  return 0;
}


int test_invalid_ciphertext()
{
  unsigned char sk_a[CRYPTO_SECRETKEYBYTES];
  unsigned char key_a[CRYPTO_BYTES], key_b[CRYPTO_BYTES];
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sendb[CRYPTO_CIPHERTEXTBYTES];
  int i;
  size_t pos;

  for(i=0; i<NTESTS; i++)
  {
    randombytes((unsigned char *)&pos, sizeof(size_t));

    //Alice generates a public key
    PQCLEAN_NAMESPACE_crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    PQCLEAN_NAMESPACE_crypto_kem_enc(sendb, key_b, pk);

    //Change some byte in the ciphertext (i.e., encapsulated key)
    sendb[pos % CRYPTO_CIPHERTEXTBYTES] ^= 23;

    //Alice uses Bobs response to get her secre key
    PQCLEAN_NAMESPACE_crypto_kem_dec(key_a, sendb, sk_a);

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
