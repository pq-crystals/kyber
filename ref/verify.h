#ifndef VERIFY_H
#define VERIFY_H

#include <stdio.h>

int PQCLEAN_NAMESPACE_verify(const unsigned char *a, const unsigned char *b, size_t len);

void PQCLEAN_NAMESPACE_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b);

#endif
