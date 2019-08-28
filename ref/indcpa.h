#ifndef INDCPA_H
#define INDCPA_H

#include <stdint.h>

void indcpa_keypair(
        uint8_t *pk,
        uint8_t *sk);

void indcpa_enc(
        uint8_t *c,
        const uint8_t *m,
        const uint8_t *pk,
        const uint8_t *coins);

void indcpa_dec(
        uint8_t *m,
        const uint8_t *c,
        const uint8_t *sk);

void indcpa_enc(uint8_t *c,
                const uint8_t *m,
                const uint8_t *pk,
                const uint8_t *coins);

void indcpa_dec(uint8_t *m,
                const uint8_t *c,
                const uint8_t *sk);


void gen_matrix(polyvec *a,
                const uint8_t *seed,
                int transposed);
#endif
