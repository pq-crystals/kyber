/* Slightly modified fastrandombytes from SUPERCOP (bench.cr.yp.to) */

#include <string.h>
#include <stdlib.h>
#include "kernelrandombytes.h"
#include "aes256ctr.h"
#include "randombytes.h"

unsigned long long randombytes_calls = 0;
unsigned long long randombytes_bytes = 0;

static unsigned int init = 0;
aes256ctr_ctx state;

#define stream_KEYBYTES 32
#define stream_OUTPUTBYTES 128
static unsigned char buf[stream_OUTPUTBYTES];
static unsigned long long pos = stream_OUTPUTBYTES;

void randombytes(unsigned char *x, size_t xlen)
{
  randombytes_calls += 1;
  randombytes_bytes += xlen;

  if (!init) {
    kernelrandombytes(buf,32);
    aes256ctr_init(&state,buf,0);
    init = 1;
  }

#ifdef SIMPLE

  while (xlen > 0) {
    if (pos == stream_OUTPUTBYTES) {
      aes256ctr_squeezeblocks(buf,1,&state);
      pos = 0;
    }
    *x++ = r[pos]; xlen -= 1;
    buf[pos++] = 0;
  }

#else /* same output but optimizing copies */

  while (xlen > 0) {
    unsigned long long ready;

    if (pos == stream_OUTPUTBYTES) {
      while (xlen > stream_OUTPUTBYTES) {
        aes256ctr_squeezeblocks(x,1,&state);
        x += stream_OUTPUTBYTES;
        xlen -= stream_OUTPUTBYTES;
      }
      if (xlen == 0) return;

      aes256ctr_squeezeblocks(buf,1,&state);
      pos = 0;
    }

    ready = stream_OUTPUTBYTES - pos;
    if (xlen <= ready) ready = xlen;
    memcpy(x,buf + pos,ready);
    memset(buf + pos,0,ready);
    x += ready;
    xlen -= ready;
    pos += ready;
  }

#endif

}
