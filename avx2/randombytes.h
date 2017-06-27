#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/random.h>


void randombytes(unsigned char *x, size_t xlen);

#endif
