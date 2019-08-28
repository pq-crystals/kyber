#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>

unsigned int rej_uniform(int16_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         unsigned int buflen);

#endif
