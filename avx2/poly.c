#include <stdio.h>
#include "poly.h"
#include "ntt.h"
#include "polyvec.h"
#include "cbd.h"
#include "fips202.h"
#include "fips202x4.h"
#include "crypto_stream.h"

extern void poly_freeze(poly *r, const poly *x);

void poly_compress(unsigned char *r, const poly *a)
{
  uint32_t t[8];
  poly d;
  unsigned int i,j,k=0;

  poly_freeze(&d,a);

  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = (((d.coeffs[i+j] << 3) + KYBER_Q/2)/KYBER_Q) & 7;

    r[k]   =  t[0]       | (t[1] << 3) | (t[2] << 6);
    r[k+1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[k+2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    k += 3;
  }

}


void poly_decompress(poly *r, const unsigned char *a)
{
  unsigned int i;
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] =  (((a[0] & 7) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+1] = ((((a[0] >> 3) & 7) * KYBER_Q)+ 4) >> 3;
    r->coeffs[i+2] = ((((a[0] >> 6) | ((a[1] << 2) & 4)) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+3] = ((((a[1] >> 1) & 7) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+4] = ((((a[1] >> 4) & 7) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+5] = ((((a[1] >> 7) | ((a[2] << 1) & 6)) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+6] = ((((a[2] >> 2) & 7) * KYBER_Q) + 4)>> 3;
    r->coeffs[i+7] = ((((a[2] >> 5)) * KYBER_Q) + 4)>> 3;
    a += 3;
  }
}


void poly_tobytes(unsigned char *r, const poly *a)
{
  int i;
  poly d;

  poly_freeze(&d, a);

  for(i=0;i<KYBER_N/8;i++)
  {
    r[13*i+ 0] =  d.coeffs[8*i+0]        & 0xff;
    r[13*i+ 1] = (d.coeffs[8*i+0] >>  8) | ((d.coeffs[8*i+1] & 0x07) << 5);
    r[13*i+ 2] = (d.coeffs[8*i+1] >>  3) & 0xff;
    r[13*i+ 3] = (d.coeffs[8*i+1] >> 11) | ((d.coeffs[8*i+2] & 0x3f) << 2);
    r[13*i+ 4] = (d.coeffs[8*i+2] >>  6) | ((d.coeffs[8*i+3] & 0x01) << 7);
    r[13*i+ 5] = (d.coeffs[8*i+3] >>  1) & 0xff;
    r[13*i+ 6] = (d.coeffs[8*i+3] >>  9) | ((d.coeffs[8*i+4] & 0x0f) << 4);
    r[13*i+ 7] = (d.coeffs[8*i+4] >>  4) & 0xff;
    r[13*i+ 8] = (d.coeffs[8*i+4] >> 12) | ((d.coeffs[8*i+5] & 0x7f) << 1);
    r[13*i+ 9] = (d.coeffs[8*i+5] >>  7) | ((d.coeffs[8*i+6] & 0x03) << 6);
    r[13*i+10] = (d.coeffs[8*i+6] >>  2) & 0xff;
    r[13*i+11] = (d.coeffs[8*i+6] >> 10) | ((d.coeffs[8*i+7] & 0x1f) << 3);
    r[13*i+12] = (d.coeffs[8*i+7] >>  5);
  }
}

void poly_frombytes(poly *r, const unsigned char *a)
{
  int i;
  for(i=0;i<KYBER_N/8;i++)
  {
    r->coeffs[8*i+0] =  a[13*i+ 0]       | (((uint32_t)a[13*i+ 1] & 0x1f) << 8);
    r->coeffs[8*i+1] = (a[13*i+ 1] >> 5) | (((uint32_t)a[13*i+ 2]       ) << 3) | (((uint32_t)a[13*i+ 3] & 0x03) << 11);
    r->coeffs[8*i+2] = (a[13*i+ 3] >> 2) | (((uint32_t)a[13*i+ 4] & 0x7f) << 6);
    r->coeffs[8*i+3] = (a[13*i+ 4] >> 7) | (((uint32_t)a[13*i+ 5]       ) << 1) | (((uint32_t)a[13*i+ 6] & 0x0f) <<  9);
    r->coeffs[8*i+4] = (a[13*i+ 6] >> 4) | (((uint32_t)a[13*i+ 7]       ) << 4) | (((uint32_t)a[13*i+ 8] & 0x01) << 12);
    r->coeffs[8*i+5] = (a[13*i+ 8] >> 1) | (((uint32_t)a[13*i+ 9] & 0x3f) << 7);
    r->coeffs[8*i+6] = (a[13*i+ 9] >> 6) | (((uint32_t)a[13*i+10]       ) << 2) | (((uint32_t)a[13*i+11] & 0x07) << 10);
    r->coeffs[8*i+7] = (a[13*i+11] >> 3) | (((uint32_t)a[13*i+12]       ) << 5);
  }
}


void poly_getnoise(poly *r,const unsigned char *seed, unsigned char nonce)
{
  unsigned char buf[KYBER_N];

  cshake128_simple(buf,KYBER_ETA*KYBER_N/4,nonce,seed,KYBER_NOISESEEDBYTES);

  cbd(r, buf);
}

void poly_getnoise4x(poly *r0, poly *r1, poly *r2, poly *r3, const unsigned char *seed, unsigned char nonce0, unsigned char nonce1, unsigned char nonce2, unsigned char nonce3)
{
  unsigned char buf0[KYBER_N];
  unsigned char buf1[KYBER_N];
  unsigned char buf2[KYBER_N];
  unsigned char buf3[KYBER_N];

  cshake128_simple4x(buf0,buf1,buf2,buf3,KYBER_N,nonce0,nonce1,nonce2,nonce3,seed,KYBER_NOISESEEDBYTES);

  cbd(r0, buf0);
  cbd(r1, buf1);
  cbd(r2, buf2);
  cbd(r3, buf3);
}


void poly_getnoise_local(poly *r,const unsigned char *seed, unsigned char nonce)
{
  unsigned char buf[KYBER_N];
#ifndef TESTVECTORS
  unsigned char n[CRYPTO_STREAM_NONCEBYTES];
  int i;

  for(i=0;i<CRYPTO_STREAM_NONCEBYTES;i++)
    n[i] = 0;
  n[3] = nonce; // The eSTREAM/SUPERCOP AES implementations set the counter to the nonce
                // Use position 3 to prevent interaction with the counter
                // independent of counter endianess etc.
                // THIS REALLY WANTS FIXING IN ALL AES-CTR IMPLEMENTATIONS IN SUPERCOP!

  crypto_stream(buf,KYBER_N,n,seed);
#else
  cshake128_simple(buf,KYBER_N,nonce,seed,KYBER_NOISESEEDBYTES);
#endif
  cbd(r, buf);
}



void poly_ntt(poly *r)
{
  mul_coefficients(r->coeffs, psis_bitrev);
  ntt(r->coeffs, omegas_bitrev);
}

void poly_invntt(poly *r)
{
  bitrev_vector(r->coeffs);
  ntt(r->coeffs, omegas_inv_bitrev);
  mul_coefficients(r->coeffs, psis_inv);
}
  

void poly_frommsg(poly *r, const unsigned char msg[KYBER_SHAREDKEYBYTES])
{
  uint32_t i,j,mask;

  for(i=0;i<KYBER_SHAREDKEYBYTES;i++)
  {
    for(j=0;j<8;j++)
    {   
      mask = -((msg[i] >> j)&1);
      r->coeffs[8*i+j] = mask & ((KYBER_Q+1)/2);
    }   
  }
}

void poly_tomsg(unsigned char msg[KYBER_SHAREDKEYBYTES], const poly *a)
{
  uint16_t t;
  int i,j;
  poly d;

  poly_freeze(&d, a);

  for(i=0;i<KYBER_SHAREDKEYBYTES;i++)
  {
    msg[i] = 0;
    for(j=0;j<8;j++)
    {
      t = (((d.coeffs[8*i+j] << 1) + KYBER_Q/2)/KYBER_Q) & 1;
      msg[i] |= t << j;
    }
  }
}
