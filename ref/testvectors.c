/* Deterministic randombytes by Daniel J. Bernstein */
/* taken from SUPERCOP (https://bench.cr.yp.to)     */

#include "api.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define NTESTS 1000


typedef uint32_t uint32;

static uint32 seed[32] = { 3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5 } ;
static uint32 in[12];
static uint32 out[8];
static int outleft = 0;

#define ROTATE(x,b) (((x) << (b)) | ((x) >> (32 - (b))))
#define MUSH(i,b) x = t[i] += (((x ^ seed[i]) + sum) ^ ROTATE(x,b));

static void surf(void)
{
  uint32 t[12]; uint32 x; uint32 sum = 0;
  int r; int i; int loop;

  for (i = 0;i < 12;++i) t[i] = in[i] ^ seed[12 + i];
  for (i = 0;i < 8;++i) out[i] = seed[24 + i];
  x = t[11];
  for (loop = 0;loop < 2;++loop) {
    for (r = 0;r < 16;++r) {
      sum += 0x9e3779b9;
      MUSH(0,5) MUSH(1,7) MUSH(2,9) MUSH(3,13)
      MUSH(4,5) MUSH(5,7) MUSH(6,9) MUSH(7,13)
      MUSH(8,5) MUSH(9,7) MUSH(10,9) MUSH(11,13)
    }
    for (i = 0;i < 8;++i) out[i] ^= t[i + 4];
  }
}

void randombytes(unsigned char *x,unsigned long long xlen)
{
  while (xlen > 0) {
    if (!outleft) {
      if (!++in[0]) if (!++in[1]) if (!++in[2]) ++in[3];
      surf();
      outleft = 8;
    }
    *x = out[--outleft];
    printf("%02x", *x);
    ++x;
    --xlen;
  }
  printf("\n");
}



int main(void)
{
  unsigned char key_a[KYBER_SYMBYTES], key_b[KYBER_SYMBYTES];
  unsigned char pk[KYBER_PUBLICKEYBYTES];
  unsigned char sendb[KYBER_CIPHERTEXTBYTES];
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  int i,j;

  for(i=0;i<NTESTS;i++)
  {
    // Key-pair generation
    crypto_kem_keypair(pk, sk_a);

    for(j=0;j<CRYPTO_PUBLICKEYBYTES;j++)
      printf("%02x",pk[j]);
    printf("\n");
    for(j=0;j<CRYPTO_SECRETKEYBYTES;j++)
      printf("%02x",sk_a[j]);
    printf("\n");

    // Encapsulation
    crypto_kem_enc(sendb, key_b, pk);

    for(j=0;j<CRYPTO_CIPHERTEXTBYTES;j++)
      printf("%02x",sendb[j]);
    printf("\n");
    for(j=0;j<CRYPTO_BYTES;j++)
      printf("%02x",key_b[j]);
    printf("\n");

    // Decapsulation
    crypto_kem_dec(key_a, sendb, sk_a);
    for(j=0;j<CRYPTO_BYTES;j++)
      printf("%02x",key_a[j]);
    printf("\n");

    for(j=0;j<CRYPTO_BYTES;j++)
    {
      if(key_a[j] != key_b[j]) 
      {
        fprintf(stderr, "ERROR\n");
        return -1;
      }
    }
  }

  return 0;
}
