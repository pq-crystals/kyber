#ifndef CBD_H
#define CBD_H

#include <stdint.h>
#include "poly.h"

void cbd(poly *r, const unsigned char *buf);
void cbdeta4(poly *r, const unsigned char *buf); /* specialized for KYBER_ETA == 4 */

#endif
