#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "api.h"
#include "kex.h"
#include "params.h"
#include "indcpa.h"
#include "poly.h"
#include "polyvec.h"
#include "cpucycles.h"
#include "speed_print.h"

#define NTESTS 10000

uint64_t t[NTESTS];
__attribute__((aligned(32)))
uint8_t seed[KYBER_SYMBYTES] = {0};

int main()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
  __attribute__((aligned(32)))
  unsigned char key[CRYPTO_BYTES] = {0};
  unsigned char kexsenda[KEX_AKE_SENDABYTES] = {0};
  unsigned char kexsendb[KEX_AKE_SENDBBYTES] = {0};
  unsigned char kexkey[KEX_SSBYTES] = {0};
  polyvec matrix[KYBER_K];
  poly ap;
#ifndef KYBER_90S
  poly bp, cp, dp;
#endif

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    gen_matrix(matrix, seed, 0);
  }
  print_results("gen_a: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_getnoise_eta2(&ap, seed, 0);
  }
  print_results("poly_getnoise_eta2: ", t, NTESTS);

#ifndef KYBER_90S
#if KYBER_ETA1 == 3
  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_getnoise_eta2(&ap, seed, 0);
    poly_getnoise_eta1_4x(&ap, &bp, &cp, &dp, seed, 0, 1, 2, 3);
  }
  print_results("poly_getnoise_eta1_4x: ", t, NTESTS);
#else
  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_getnoise_eta2(&ap, seed, 0);
    poly_getnoise_eta2_4x(&ap, &bp, &cp, &dp, seed, 0, 1, 2, 3);
  }
  print_results("poly_getnoise_eta2_4x: ", t, NTESTS);
#endif 
#endif 

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_ntt(&ap);
  }
  print_results("NTT: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_invntt_tomont(&ap);
  }
  print_results("INVNTT: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    polyvec_pointwise_acc_montgomery(&ap, &matrix[0], &matrix[1]);
  }
  print_results("polyvec_basemul_acc_montgomery: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_compress(ct,&ap);
  }
  print_results("poly_compress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    poly_decompress(&ap,ct);
  }
  print_results("poly_decompress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    polyvec_compress(ct,&matrix[0]);
  }
  print_results("polyvec_compress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    polyvec_decompress(&matrix[0],ct);
  }
  print_results("polyvec_decompress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    indcpa_keypair(pk, sk);
  }
  print_results("indcpa_keypair: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    indcpa_enc(ct, key, pk, seed);
  }
  print_results("indcpa_enc: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    indcpa_dec(key, ct, sk);
  }
  print_results("indcpa_dec: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_keypair(pk, sk);
  }
  print_results("kyber_keypair: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_enc(ct, key, pk);
  }
  print_results("kyber_encaps: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    crypto_kem_dec(key, ct, sk);
  }
  print_results("kyber_decaps: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_uake_initA(kexsenda, key, sk, pk);
  }
  print_results("kex_uake_initA: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_uake_sharedB(kexsendb, kexkey, kexsenda, sk);
  }
  print_results("kex_uake_sharedB: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_uake_sharedA(kexkey, kexsendb, key, sk);
  }
  print_results("kex_uake_sharedA: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_ake_initA(kexsenda, key, sk, pk);
  }
  print_results("kex_ake_initA: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_ake_sharedB(kexsendb, kexkey, kexsenda, sk, pk);
  }
  print_results("kex_ake_sharedB: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    kex_ake_sharedA(kexkey, kexsendb, key, sk, sk);
  }
  print_results("kex_ake_sharedA: ", t, NTESTS);

  return 0;
}
