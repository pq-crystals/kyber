#ifndef VERIFY_H
#define VERIFY_H

#include <stddef.h>
#include <stdint.h>

uint8_t PQCLEAN_NAMESPACE_verify(const uint8_t *a, const uint8_t *b, size_t len);

void PQCLEAN_NAMESPACE_cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

#endif
