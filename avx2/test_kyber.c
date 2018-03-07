#include "api.h"
#include "poly.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 10000

int test_keys()
{
  unsigned char key_a[KYBER_SYMBYTES], key_b[KYBER_SYMBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_CIPHERTEXTBYTES];
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(sendb, key_b, pk);
  
    //Alice uses Bobs response to get her secret key
    crypto_kem_dec(key_a, sendb, sk_a);

    if(memcmp(key_a, key_b, KYBER_SYMBYTES))
      printf("ERROR keys\n");
  }

  return 0;
}


int test_invalid_sk_a()
{
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  unsigned char key_a[KYBER_SYMBYTES], key_b[KYBER_SYMBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_CIPHERTEXTBYTES];
  int i;

  for(i=0; i<NTESTS; i++)
  {
    //Alice generates a public key
    crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(sendb, key_b, pk);

    //Replace secret key with random values
    randombytes(sk_a, KYBER_SECRETKEYBYTES);

  
    //Alice uses Bobs response to get her secre key
    crypto_kem_dec(key_a, sendb, sk_a);
    
    if(!memcmp(key_a, key_b, KYBER_SYMBYTES))
      printf("ERROR invalid sk_a\n");
  }

  return 0;
}


int test_invalid_ciphertext()
{
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  unsigned char key_a[KYBER_SYMBYTES], key_b[KYBER_SYMBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_CIPHERTEXTBYTES];
  int i;
  size_t pos;

  for(i=0; i<NTESTS; i++)
  {
    randombytes((unsigned char *)&pos, sizeof(size_t));

    //Alice generates a public key
    crypto_kem_keypair(pk, sk_a);

    //Bob derives a secret key and creates a response
    crypto_kem_enc(sendb, key_b, pk);

    //Change some byte in the ciphertext (i.e., encapsulated key)
    sendb[pos % KYBER_CIPHERTEXTBYTES] ^= 23;
  
    //Alice uses Bobs response to get her secre key
    crypto_kem_dec(key_a, sendb, sk_a);

    if(!memcmp(key_a, key_b, KYBER_SYMBYTES))
      printf("ERROR invalid ciphertext\n");
  }

  return 0;
}

int main(void)
{
  test_keys();
  test_invalid_sk_a();
  test_invalid_ciphertext();
  
  printf("KYBER_SECRETKEYBYTES:  %d\n",KYBER_SECRETKEYBYTES);
  printf("KYBER_PUBLICKEYBYTES:  %d\n",KYBER_PUBLICKEYBYTES);
  printf("KYBER_CIPHERTEXTBYTES: %d\n",KYBER_CIPHERTEXTBYTES);

  return 0;
}
