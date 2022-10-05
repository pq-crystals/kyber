#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "kem.h"
#include "kex.h"
#include "params.h"
#include "indcpa.h"
#include "polyvec.h"
#include "poly.h"
#include "randombytes.h"

#define NTESTS 1000


static inline uint64_t cpucycles(void) {
  uint64_t result;

  __asm__ volatile ("rdtsc; shlq $32,%%rdx; orq %%rdx,%%rax"
    : "=a" (result) : : "%rdx");

  return result;
}

static uint64_t cpucycles_overhead(void) {
  uint64_t t0, t1, overhead = -1LL;
  unsigned int i;

  for(i=0;i<100000;i++) {
    t0 = cpucycles();
    __asm__ volatile ("");
    t1 = cpucycles();
    if(t1 - t0 < overhead)
      overhead = t1 - t0;
  }

  return overhead;
}
 
static int cmp_uint64(const void *a, const void *b) {
  if(*(uint64_t *)a < *(uint64_t *)b) return -1;
  if(*(uint64_t *)a > *(uint64_t *)b) return 1;
  return 0;
}

static uint64_t median(uint64_t *l, size_t llen) {
  qsort(l,llen,sizeof(uint64_t),cmp_uint64);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}


static void print_bench(char *s, int k, uint64_t *t, size_t tlen, int enc)
{
  static uint64_t overhead = -1;
  size_t i;

  if(tlen < 2) {
    fprintf(stderr, "ERROR: Need a least two cycle counts!\n");
    return;
  }

  if(overhead  == (uint64_t)-1)
    overhead = cpucycles_overhead();

  tlen--;
  for(i=0;i<tlen;++i)
    t[i] = t[i+1] - t[i] - overhead;

  if(enc)
  {
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("oneref}{$%lu$}\n",median(t, tlen));
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("tworef}{$%lu$}\n",2*median(t, tlen));
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("Xref}{$%lu$}\n",10*median(t, tlen));
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("Cref}{$%lu$}\n",100*median(t, tlen));
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("Mref}{$%lu$}\n",1000*median(t, tlen));
  }
  else
  {
    printf("\\newcommand{%s", s);
    if(k==2) printf("low");
    if(k==3) printf("mid");
    if(k==4) printf("high");
    printf("ref}{$%lu$}\n",median(t, tlen));
  }
}

int main()
{
  unsigned int i;
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key[CRYPTO_BYTES];
  uint64_t t[NTESTS];

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_keypair(pk, sk);
  }
  print_bench("\\gencyc", KYBER_K, t, NTESTS, 0);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_enc(ct, key, pk);
  }
  print_bench("\\enccyc", KYBER_K, t, NTESTS, 1);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_dec(key, ct, sk);
  }
  print_bench("\\deccyc", KYBER_K, t, NTESTS, 0);

  return 0;
}
