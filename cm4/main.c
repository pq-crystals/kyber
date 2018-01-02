#include <stdlib.h>

#include "cpucycles.h"
#include "kex.h"
#include "ntt.h"
#include "randombytes.h"  // for randombytes

#define NTESTS 10

unsigned long __cycle_start;

inline void START_CYCLE_COUNT() { __cycle_start = cpucycles(); }
inline unsigned long END_CYCLE_COUNT() {
  return (cpucycles() - __cycle_start);
}

static int cmp_llu(const void *a, const void *b) {
  if (*(unsigned int *)a < *(unsigned int *)b) return -1;
  if (*(unsigned int *)a > *(unsigned int *)b) return 1;
  return 0;
}

static unsigned int median(unsigned int *l, int llen) {
  qsort(l, llen, sizeof(unsigned int), cmp_llu);
  if (llen % 2)
    return l[llen / 2];
  else
    return (l[llen / 2 - 1] + l[llen / 2]) / 2;
}

static unsigned int average(unsigned int *t, int tlen) {
  unsigned int acc = 0;
  int i;
  for (i = 0; i < tlen; i++) acc += t[i];
  return acc / (tlen);
}

void print_results(const char *s, unsigned int *t, int tlen) {
  printf("%s\n", s);
  printf("\tmedian: %u\n", median(t, tlen));
  printf("\taverage: %u\n", average(t, tlen));
  printf("\n");
}

int main() {
  cpucycles_init();

  int i;

  unsigned int
      t[NTESTS];  // = (unsigned int*) malloc(sizeof(unsigned int)*NTESTS);
  unsigned char sk_a[KYBER_SECRETKEYBYTES];
  unsigned char key_a[32], key_b[32];
  unsigned char *senda = (unsigned char *)malloc(KYBER_PUBLICKEYBYTES);
  unsigned char *sendb = (unsigned char *)malloc(KYBER_CIPHERTEXTBYTES);

  uint16_t *a = (uint16_t *)malloc(KYBER_N * sizeof(uint16_t));

  for (i = 0; i < NTESTS; i++) {
    START_CYCLE_COUNT();
    ntt(a);
    t[i] = END_CYCLE_COUNT();
  }
  print_results("ntt: ", t, NTESTS);

  for (i = 0; i < NTESTS; i++) {
    START_CYCLE_COUNT();
    crypto_kem_keypair(senda, sk_a);
    t[i] = END_CYCLE_COUNT();
  }
  print_results("kyber_keypair: ", t, NTESTS);

  printf("senda: ");
  for (i = 0; i < 32; i++) printf("%02x", senda[i]);
  printf("\n");

  printf("sk_a: ");
  for (i = 0; i < 32; i++) printf("%02x", sk_a[i]);
  printf("\n");

  for (i = 0; i < NTESTS; i++) {
    START_CYCLE_COUNT();
    crypto_kem_enc(sendb, key_b, senda);
    t[i] = END_CYCLE_COUNT();
  }
  print_results("kyber_encaps:  ", t, NTESTS);

  printf("sendb: ");
  for (i = 0; i < 32; i++) printf("%02x", sendb[i]);
  printf("\n");

  printf("key_b: ");
  for (i = 0; i < 32; i++) printf("%02x", key_b[i]);
  printf("\n");

  for (i = 0; i < NTESTS; i++) {
    START_CYCLE_COUNT();
    crypto_kem_dec(key_a, sendb, sk_a);
    t[i] = END_CYCLE_COUNT();
  }
  print_results("kyber_decaps:  ", t, NTESTS);

  printf("key_a: ");
  for (i = 0; i < 32; i++) printf("%02x", key_a[i]);
  printf("\n");

  free(a);
  free(senda);
  free(sendb);

  while (1) {
  }
  return 0;
}

