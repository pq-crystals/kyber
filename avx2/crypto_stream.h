#ifndef CRYPTO_STREAM_H
#define CRYPTO_STREAM_H

#define CRYPTO_STREAM_KEYBYTES 32
#define CRYPTO_STREAM_NONCEBYTES 16
#define crypto_stream crypto_stream_aes256ctr

int crypto_stream_aes256ctr(unsigned char *c,unsigned long long clen, const unsigned char *n, const unsigned char *k);

#endif
