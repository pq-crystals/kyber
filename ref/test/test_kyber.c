#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../kem.h"
#include "../randombytes.h"

#define NTESTS 1 // test count

void run_test(int test_idx)
{
  // KeyGen
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];

  crypto_kem_keypair(pk, sk);
  
  /* fprintf(fout, "Test #%d\n", test_idx+1);
  fprintf(fout, "KeyGen Stage:\n- Input: None\n- Output:\n");
  fprintf(fout, "* Public Key: ");
  for(size_t i=0; i<CRYPTO_PUBLICKEYBYTES; i++) fprintf(fout, "%02x", pk[i]);
  fprintf(fout, "\n* Secret Key: ");
  for(size_t i=0; i<CRYPTO_SECRETKEYBYTES; i++) fprintf(fout, "%02x", sk[i]);
  fprintf(fout, "\n\n"); */

  // Encapsulation Stage (A send and encrypt ct)
  crypto_kem_enc(ct, key_b, pk);

  /* fprintf(fout, "Encapsulation Stage (A):\n- Input: pk\n- Output:\n");
  fprintf(fout, "* Ciphertext: ");
  for(size_t i=0; i<CRYPTO_CIPHERTEXTBYTES; i++) fprintf(fout, "%02x", ct[i]);
  fprintf(fout, "\n* Shared Secret: ");
  for(size_t i=0; i<CRYPTO_BYTES; i++) fprintf(fout, "%02x", key_b[i]);
  fprintf(fout, "\n\n"); */

  // Make secret key sk or ciphertext ct invalid to test
  // ct[0] ^= 0xFF; // sk[0] ^= 0xFF; 

  // Decapsulation Stage (B receive and decrypt ct)
  crypto_kem_dec(key_a, ct, sk);
  
  // fprintf(fout, "Decapsulation Stage (B):\n- Input: ct, sk\n- Output: \n* Shared Secret: ");
  // for(size_t i=0; i<CRYPTO_BYTES; i++) fprintf(fout, "%02x", key_a[i]);
  // fprintf(fout, "\n");

  // Compare shared secret
  /* if(memcmp(key_a, key_b, CRYPTO_BYTES)) {
    printf("\nERROR: shared secret mismatch!\n");
    fprintf(fout, "\nResult: shared secret mismatch!\n");
  } else {
    printf("\nSUCCESS: shared secret match!\n");
    fprintf(fout, "\nResult: shared secret match!\n");
  }
  fprintf(fout, "\n"); */
  
  (void)test_idx;
}

// reference test functions:
/* static int test_invalid_sk_a(void)
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];

  //Alice generates a public key
  crypto_kem_keypair(pk, sk);

  //Bob derives a secret key and creates a response
  crypto_kem_enc(ct, key_b, pk);

  //Replace secret key with random values
  randombytes(sk, CRYPTO_SECRETKEYBYTES);

  //Alice uses Bobs response to get her shared key
  crypto_kem_dec(key_a, ct, sk);

  if(!memcmp(key_a, key_b, CRYPTO_BYTES)) {
    printf("ERROR: invalid sk\n");
    return 1;
  }

  return 0;
}

static int test_invalid_ciphertext(void)
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];
  uint8_t b;
  size_t pos;

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

  if(!memcmp(key_a, key_b, CRYPTO_BYTES)) {
    printf("ERROR: invalid ciphertext\n");
    return 1;
  }

  return 0;
} */

int main(void)
{
  
  for (int test = 0; test < NTESTS; ++test) {
    run_test(test);
  }

  // Print testing information
  printf("\n[Testing Information - %d runs]\n\n", NTESTS);
  timing_info_t t = print_timing_info();
  printf("Average KeyGen time: %.6fs (%.2f ms)\n", t.keygen / NTESTS, (t.keygen / NTESTS) * 1000);
  printf("Average Encapsulation time: %.6fs (%.2f ms)\n", t.encap / NTESTS, (t.encap / NTESTS) * 1000);
  printf("Average Decapsulation time: %.6fs (%.2f ms)\n", t.decap / NTESTS, (t.decap / NTESTS) * 1000);
  printf("Average all time (NIST compliance): %.6fs (%.2f ms)\n", t.all / NTESTS, (t.all / NTESTS) * 1000);
  printf("Public key bytes = %d\n", CRYPTO_PUBLICKEYBYTES);
  printf("Secret key bytes = %d\n", CRYPTO_SECRETKEYBYTES);
  printf("Ciphertext bytes = %d\n", CRYPTO_CIPHERTEXTBYTES);
  printf("Shared secret bytes = %d\n", CRYPTO_BYTES);

  return 0;
}