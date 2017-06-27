#include "cbd.h"

static uint32_t load_littleendian(const unsigned char *x)
{
  return x[0] | (((uint32_t)x[1]) << 8) | (((uint32_t)x[2]) << 16) | (((uint32_t)x[3]) << 24);
}


void cbd(poly *r, const unsigned char *buf)
{
#if KYBER_K != 4
#error "poly_getnoise in poly.c only supports k=4"
#endif

  uint32_t t,d, a[4], b[4];
  int i,j;

  for(i=0;i<KYBER_N/4;i++)
  {
    t = load_littleendian(buf+4*i);
    d = 0;
    for(j=0;j<4;j++)
      d += (t >> j) & 0x11111111;

    a[0] =  d & 0xf;
    b[0] = (d >>  4) & 0xf;
    a[1] = (d >>  8) & 0xf;
    b[1] = (d >> 12) & 0xf;
    a[2] = (d >> 16) & 0xf;
    b[2] = (d >> 20) & 0xf;
    a[3] = (d >> 24) & 0xf;
    b[3] = (d >> 28);

    r->coeffs[4*i+0] = a[0] + KYBER_Q - b[0];
    r->coeffs[4*i+1] = a[1] + KYBER_Q - b[1];
    r->coeffs[4*i+2] = a[2] + KYBER_Q - b[2];
    r->coeffs[4*i+3] = a[3] + KYBER_Q - b[3];
  }
}
