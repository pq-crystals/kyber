//
//  rng.h
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//

#ifndef rng_h
#define rng_h

#include <stddef.h>
#include <stdint.h>

#define RNG_SUCCESS      0
#define RNG_BAD_MAXLEN  -1
#define RNG_BAD_OUTBUF  -2
#define RNG_BAD_REQ_LEN -3

typedef struct {
    uint8_t   buffer[16];
    int             buffer_pos;
    unsigned long   length_remaining;
    uint8_t   key[32];
    uint8_t   ctr[16];
} AES_XOF_struct;

typedef struct {
    uint8_t   Key[32];
    uint8_t   V[16];
    int             reseed_counter;
} AES256_CTR_DRBG_struct;


void
AES256_CTR_DRBG_Update(uint8_t *provided_data,
                       uint8_t *Key,
                       uint8_t *V);

int
seedexpander_init(AES_XOF_struct *ctx,
                  uint8_t *seed,
                  uint8_t *diversifier,
                  unsigned long maxlen);

int
seedexpander(AES_XOF_struct *ctx, uint8_t *x, unsigned long xlen);

void
randombytes_init(uint8_t *entropy_input,
                 uint8_t *personalization_string,
                 int security_strength);

int
randombytes(uint8_t *x, unsigned long long xlen);

#endif /* rng_h */
