#include "api.h"
#include "kex.h"
#include "poly.h"
#include "polyvec.h"
#include "cpucycles.h"
#include <stdlib.h>
#include <stdio.h>

#define NTESTS 2000

extern void gen_matrix(polyvec *a, const unsigned char *seed, int transposed);

unsigned long long t[NTESTS], overhead;
unsigned char seed[32] = {0};

static int cmp_llu(const void *a, const void*b)
{
  if(*(unsigned long long *)a < *(unsigned long long *)b) return -1;
  if(*(unsigned long long *)a > *(unsigned long long *)b) return 1;
  return 0;
}

static unsigned long long median(unsigned long long *l, size_t llen)
{
  qsort(l,llen,sizeof(unsigned long long),cmp_llu);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}

static unsigned long long average(unsigned long long *t, size_t tlen)
{
  unsigned long long acc=0;
  size_t i;
  for(i=0;i<tlen;i++)
    acc += t[i];
  return acc/(tlen);
}

static void print_results(const char *s, unsigned long long *t, size_t tlen)
{
  size_t i;
  printf("%s", s);
  for(i=0;i<tlen-1;i++)
  {
    t[i] = t[i+1] - t[i] - overhead;
  }
  printf("\n");
  printf("median: %llu\n", median(t, tlen-1));
  printf("average: %llu\n", average(t, tlen-1));
  printf("\n");
}

int main()
{
  unsigned char sk_a[CRYPTO_SECRETKEYBYTES];
  unsigned char pk_a[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk_b[CRYPTO_SECRETKEYBYTES];
  unsigned char pk_b[CRYPTO_PUBLICKEYBYTES];

  unsigned char eska[CRYPTO_SECRETKEYBYTES];
  unsigned char tk[CRYPTO_BYTES];

  unsigned char key_a[32], key_b[32];
  unsigned char* senda = (unsigned char*) malloc(NTESTS*CRYPTO_PUBLICKEYBYTES);
  unsigned char* sendb = (unsigned char*) malloc(NTESTS*CRYPTO_CIPHERTEXTBYTES);

  unsigned char* kexsenda = (unsigned char*) malloc(NTESTS*KEX_AKE_SENDABYTES);
  unsigned char* kexsendb = (unsigned char*) malloc(NTESTS*KEX_AKE_SENDBBYTES);

  poly ap;
  polyvec matrix[KYBER_K];
  int i;

  overhead = cpucycles_overhead();
  // XXX: ramp up frequency
  for(i = 0; i < 100000; i++)
    poly_ntt(&ap);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    poly_ntt(&ap);
  }
  print_results("NTT:           ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    poly_invntt(&ap);
  }
  print_results("INVNTT:        ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    gen_matrix(matrix, seed, 0);
  }
  print_results("gen_a:         ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    poly_getnoise(&ap, seed, 0);
  }
  print_results("poly_getnoise: ", t, NTESTS);


  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_keypair(senda+i*CRYPTO_PUBLICKEYBYTES, sk_a);
  }
  print_results("kyber_keypair: ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_enc(sendb+i*CRYPTO_CIPHERTEXTBYTES, key_b, senda+i*CRYPTO_PUBLICKEYBYTES);
  }
  print_results("kyber_encaps:  ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_dec(key_a, sendb+i*CRYPTO_CIPHERTEXTBYTES, sk_a);
  }
  print_results("kyber_decaps:  ", t, NTESTS);


  /* Generating static keys for AKE */
  crypto_kem_keypair(pk_a, sk_a); // Generate static key for Alice
  crypto_kem_keypair(pk_b, sk_b); // Generate static key for Bob



  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_uake_initA(kexsenda+i*KEX_AKE_SENDABYTES, tk, eska, pk_b); // Run by Alice
  }
  print_results("kex_uake_initA: ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_uake_sharedB(kexsendb+i*KEX_AKE_SENDBBYTES, key_b, kexsenda+i*KEX_AKE_SENDABYTES, sk_b); // Run by Bob
  }
  print_results("kex_uake_sharedB:  ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_uake_sharedA(key_a, kexsendb+i*KEX_AKE_SENDBBYTES, tk, eska); // Run by Alice
  }
  print_results("kex_uake_sharedA:  ", t, NTESTS);



  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_ake_initA(kexsenda+i*KEX_AKE_SENDABYTES, tk, eska, pk_b); // Run by Alice
  }
  print_results("kex_ake_initA: ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_ake_sharedB(kexsendb+i*KEX_AKE_SENDBBYTES, key_b, kexsenda+i*KEX_AKE_SENDABYTES, sk_b, pk_a); // Run by Bob
  }
  print_results("kex_ake_sharedB:  ", t, NTESTS);

  for(i=0; i<NTESTS; i++)
  {
    t[i] = cpucycles();
    kex_ake_sharedA(key_a, kexsendb+i*KEX_AKE_SENDBBYTES, tk, eska, sk_a); // Run by Alice
  }
  print_results("kex_ake_sharedA:  ", t, NTESTS);

  // Cleaning
  free(senda);
  free(sendb);
  free(kexsenda);
  free(kexsendb);

  return 0;
}
