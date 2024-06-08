#ifndef VERIFY_H
#define VERIFY_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"

#define verify KYBER_NAMESPACE(verify)
int verify(const uint8_t *a, const uint8_t *b, size_t len);

#define cmov KYBER_NAMESPACE(cmov)
void cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

#define cmov_int16 KYBER_NAMESPACE(cmov_int16)
int16_t cmov_int16(int16_t a, int16_t b, uint16_t select);

#endif
