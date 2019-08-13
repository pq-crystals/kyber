#ifndef NTT_H
#define NTT_H

#include <stdint.h>

extern const int16_t PQCLEAN_NAMESPACE_zetas[128];
extern const int16_t PQCLEAN_NAMESPACE_zetasinv[128];

void PQCLEAN_NAMESPACE_ntt(int16_t *poly);
void PQCLEAN_NAMESPACE_invntt(int16_t *poly);
void PQCLEAN_NAMESPACE_basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

#endif
