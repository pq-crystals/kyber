#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "randombytes.h"

#define _GNU_SOURCE

static int fd = -1;
static void kernelrandombytes_fallback(unsigned char *buf,
                                       unsigned long long  len)
{
  int i;

  if(fd == -1) {
    while(1) {
      fd = open("/dev/urandom", O_RDONLY);
      if(fd != -1) break;
      sleep(1);
    }
  }

  while(len > 0) {
    if(len < 1048576) i = len; else i = 1048576;

    i = read(fd, buf, i);
    if(i < 1) {
      sleep(1);
      continue;
    }

    buf += i;
    len -= i;
  }
}

#ifdef SYS_getrandom

void kernelrandombytes(unsigned char *buf, unsigned long long len) {
  unsigned long long d = 0;
  int r;

  while(d < len) {
    errno = 0;
    r = syscall(SYS_getrandom, buf, len - d, 0);
    if(r < 0) {
      if(errno == EINTR) continue;
      kernelrandombytes_fallback(buf, len);
      return;
    }
    buf += r;
    d += r;
  }
}

#else

void kernelrandombytes(unsigned char *buf, unsigned long long len) {
  kernelrandombytes_fallback(buf, len);
}

#endif

