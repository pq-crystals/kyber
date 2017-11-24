#include <stdio.h>
#include <string.h>

#include "kex.h"

int main(void)
{
  unsigned char pkb[KYBER_PUBLICKEYBYTES];
  unsigned char skb[KYBER_SECRETKEYBYTES];
  
  unsigned char pka[KYBER_PUBLICKEYBYTES]; 
  unsigned char ska[KYBER_SECRETKEYBYTES];

  unsigned char eska[KYBER_SECRETKEYBYTES];

  unsigned char uake_senda[KYBER_UAKE_SENDABYTES];
  unsigned char uake_sendb[KYBER_UAKE_SENDBBYTES];
  
  unsigned char ake_senda[KYBER_AKE_SENDABYTES];
  unsigned char ake_sendb[KYBER_AKE_SENDBBYTES];

  unsigned char tk[KYBER_SYMBYTES];
  unsigned char ka[KYBER_SYMBYTES];
  unsigned char kb[KYBER_SYMBYTES];
  unsigned char zero[KYBER_SYMBYTES];
  int i;

  for(i=0;i<KYBER_SYMBYTES;i++)
    zero[i] = 0;

  crypto_kem_keypair(pkb, skb); // Generate static key for Bob
  
  crypto_kem_keypair(pka, ska); // Generate static key for Alice


  // Perform unilaterally authenticated key exchange
  
  kyber_uake_initA(uake_senda, tk, eska, pkb); // Run by Alice

  kyber_uake_sharedB(uake_sendb, kb, uake_senda, skb); // Run by Bob

  kyber_uake_sharedA(ka, uake_sendb, tk, eska); // Run by Alice

  if(memcmp(ka,kb,KYBER_SYMBYTES))
    printf("Error in UAKE\n");

  if(!memcmp(ka,zero,KYBER_SYMBYTES))
    printf("Error: UAKE produces zero key\n");
  
  // Perform mutually authenticated key exchange
  
  kyber_ake_initA(ake_senda, tk, eska, pkb); // Run by Alice

  kyber_ake_sharedB(ake_sendb, kb, ake_senda, skb, pka); // Run by Bob

  kyber_ake_sharedA(ka, ake_sendb, tk, eska, ska); // Run by Alice

  if(memcmp(ka,kb,KYBER_SYMBYTES))
    printf("Error in AKE\n");
  
  if(!memcmp(ka,zero,KYBER_SYMBYTES))
    printf("Error: AKE produces zero key\n");


  printf("KYBER_UAKE_SENDABYTES: %d\n",KYBER_UAKE_SENDABYTES);
  printf("KYBER_UAKE_SENDBBYTES: %d\n",KYBER_UAKE_SENDBBYTES);

  printf("KYBER_AKE_SENDABYTES: %d\n",KYBER_AKE_SENDABYTES);
  printf("KYBER_AKE_SENDBBYTES: %d\n",KYBER_AKE_SENDBBYTES);

  return 0;
}
