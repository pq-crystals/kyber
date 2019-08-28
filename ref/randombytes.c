#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "randombytes.h"
#include <sys/syscall.h>
#include <stddef.h>
#include <stdint.h>

#define _GNU_SOURCE

static int fd = -1;
static void randombytes_fallback(uint8_t *x, size_t xlen)
{
  ssize_t i = 0;

  if (fd == -1) {
    for (;;) {
      fd = open("/dev/urandom",O_RDONLY);
      if (fd != -1) break;
      sleep(1);
    }
  }

  while (xlen > 0) {
    if (xlen < 1048576)
        i = (ssize_t)xlen;
    else i = 1048576;

    i = read(fd, x, (size_t)i);
    if (i < 1) {
      sleep(1);
      continue;
    }

    x += i;
    xlen -= (size_t)i;
  }
}

#ifdef SYS_getrandom
void randombytes(uint8_t *buf,size_t buflen)
{
  size_t d = 0;
  long r;

  while(d<buflen)
  {
    errno = 0;
    r = syscall(SYS_getrandom, buf, buflen - d, 0);
    if(r < 0)
    {
      if (errno == EINTR) continue;
      randombytes_fallback(buf, buflen);
      return;
    }
    buf += r;
    d += (size_t)r;
  }
}
#else
void randombytes(uint8_t *buf,size_t buflen)
{
  randombytes_fallback(buf,buflen);
}
#endif

