CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -mavx2 -mbmi2 -mpopcnt -maes \
  -march=native -mtune=native -O3 -fomit-frame-pointer
NISTFLAGS += -Wno-unused-result -mavx2 -mbmi2 -mpopcnt -maes \
  -march=native -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm

SOURCES = kem.c indcpa.c polyvec.c poly.c fq.S shuffle.S ntt.S invntt.S \
  basemul.S consts.c rejsample.c cbd.c verify.c
SOURCESKECCAK   = $(SOURCES) fips202.c fips202x4.c symmetric-shake.c \
  keccak4x/KeccakP-1600-times4-SIMD256.o
SOURCESNINETIES = $(SOURCES) aes256ctr.c
HEADERS = params.h align.h kem.h indcpa.h polyvec.h poly.h reduce.h fq.inc shuffle.inc \
  ntt.h consts.h rejsample.h cbd.h verify.h symmetric.h randombytes.h
HEADERSKECCAK   = $(HEADERS) fips202.h fips202x4.h
HEADERSNINETIES = $(HEADERS) sha2.h aes256ctr.h

.PHONY: all shared clean

all: \
  test_kyber512 \
  test_kyber768 \
  test_kyber1024 \
  test_kex512 \
  test_kex768 \
  test_kex1024 \
  test_vectors512 \
  test_vectors768 \
  test_vectors1024 \
  test_kyber512-90s \
  test_kyber768-90s \
  test_kyber1024-90s \
  test_kex512-90s \
  test_kex768-90s \
  test_kex1024-90s \
  test_vectors512-90s \
  test_vectors768-90s \
  test_vectors1024-90s \
  speed

speed: \
  test_speed512 \
  test_speed768 \
  test_speed1024 \
  test_speed512-90s \
  test_speed768-90s \
  test_speed1024-90s

shared: \
  libpqcrystals_kyber512_avx2.so \
  libpqcrystals_kyber768_avx2.so \
  libpqcrystals_kyber1024_avx2.so \
  libpqcrystals_kyber512-90s_avx2.so \
  libpqcrystals_kyber768-90s_avx2.so \
  libpqcrystals_kyber1024-90s_avx2.so \
  libpqcrystals_fips202_ref.so \
  libpqcrystals_fips202x4_avx2.so \
  libpqcrystals_aes256ctr_avx2.so

keccak4x/KeccakP-1600-times4-SIMD256.o: \
  keccak4x/KeccakP-1600-times4-SIMD256.c \
  keccak4x/KeccakP-1600-times4-SnP.h \
  keccak4x/KeccakP-1600-unrolling.macros \
  keccak4x/KeccakP-SIMD256-config.h \
  keccak4x/KeccakP-align.h \
  keccak4x/KeccakP-brg_endian.h
	$(CC) $(CFLAGS) -c $< -o $@

libpqcrystals_fips202_ref.so: fips202.c fips202.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

libpqcrystals_fips202x4_avx2.so: fips202x4.c fips202x4.h \
  keccak4x/KeccakP-1600-times4-SIMD256.c \
  keccak4x/KeccakP-1600-times4-SnP.h \
  keccak4x/KeccakP-1600-unrolling.macros \
  keccak4x/KeccakP-SIMD256-config.h \
  keccak4x/KeccakP-align.h \
  keccak4x/KeccakP-brg_endian.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $< keccak4x/KeccakP-1600-times4-SIMD256.c

libpqcrystals_aes256ctr_avx2.so: aes256ctr.c aes256ctr.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

libpqcrystals_kyber512_avx2.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=2 $(SOURCES) \
	  symmetric-shake.c -o libpqcrystals_kyber512_avx2.so

libpqcrystals_kyber768_avx2.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=3 $(SOURCES) \
	  symmetric-shake.c -o libpqcrystals_kyber768_avx2.so

libpqcrystals_kyber1024_avx2.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=4 $(SOURCES) \
	  symmetric-shake.c -o libpqcrystals_kyber1024_avx2.so

test_kyber512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber512

test_kyber768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber768

test_kyber1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber1024

test_kex512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex512

test_kex768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex768

test_kex1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex1024

test_vectors512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) test_vectors.c -o test_vectors512

test_vectors768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) test_vectors.c -o test_vectors768

test_vectors1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) test_vectors.c -o test_vectors1024

test_speed512: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) cpucycles.c speed_print.c kex.c test_speed.c -o test_speed512

test_speed768: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) cpucycles.c speed_print.c kex.c test_speed.c -o test_speed768

test_speed1024: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) cpucycles.c speed_print.c kex.c test_speed.c -o test_speed1024

libpqcrystals_kyber512-90s_avx2.so: $(SOURCES) $(HEADERS)
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=2 -DKYBER_90S $(SOURCES) -o libpqcrystals_kyber512-90s_avx2.so

libpqcrystals_kyber768-90s_avx2.so: $(SOURCES) $(HEADERS)
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=3 -DKYBER_90S $(SOURCES) -o libpqcrystals_kyber768-90s_avx2.so

libpqcrystals_kyber1024-90s_avx2.so: $(SOURCES) $(HEADERS)
	$(CC) -shared -fpic $(CFLAGS) -DKYBER_K=4 -DKYBER_90S $(SOURCES) -o libpqcrystals_kyber1024-90s_avx2.so

test_kyber512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber512-90s $(LDFLAGS) -lcrypto

test_kyber768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber768-90s $(LDFLAGS) -lcrypto

test_kyber1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber1024-90s $(LDFLAGS) -lcrypto

test_kex512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex512-90s $(LDFLAGS) -lcrypto

test_kex768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex768-90s $(LDFLAGS) -lcrypto

test_kex1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex1024-90s $(LDFLAGS) -lcrypto

test_vectors512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) test_vectors.c -o test_vectors512-90s $(LDFLAGS) -lcrypto

test_vectors768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) test_vectors.c -o test_vectors768-90s $(LDFLAGS) -lcrypto

test_vectors1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) test_vectors.c -o test_vectors1024-90s $(LDFLAGS) -lcrypto

test_speed512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h fips202.c fips202.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) cpucycles.c kex.c fips202.c speed_print.c test_speed.c -o test_speed512-90s $(LDFLAGS) -lcrypto

test_speed768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h fips202.c fips202.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) cpucycles.c kex.c fips202.c speed_print.c test_speed.c -o test_speed768-90s $(LDFLAGS) -lcrypto

test_speed1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c kex.c kex.h fips202.c fips202.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) cpucycles.c kex.c fips202.c speed_print.c test_speed.c -o test_speed1024-90s $(LDFLAGS) -lcrypto

clean:
	-$(RM) -rf *.o *.a *.so
	-$(RM) -rf test_kyber512
	-$(RM) -rf test_kyber768
	-$(RM) -rf test_kyber1024
	-$(RM) -rf test_kex512
	-$(RM) -rf test_kex768
	-$(RM) -rf test_kex1024
	-$(RM) -rf test_vectors512
	-$(RM) -rf test_vectors768
	-$(RM) -rf test_vectors1024
	-$(RM) -rf test_speed512
	-$(RM) -rf test_speed768
	-$(RM) -rf test_speed1024
	-$(RM) -rf test_kyber512-90s
	-$(RM) -rf test_kyber768-90s
	-$(RM) -rf test_kyber1024-90s
	-$(RM) -rf test_kex512-90s
	-$(RM) -rf test_kex768-90s
	-$(RM) -rf test_kex1024-90s
	-$(RM) -rf test_vectors512-90s
	-$(RM) -rf test_vectors768-90s
	-$(RM) -rf test_vectors1024-90s
	-$(RM) -rf test_speed512-90s
	-$(RM) -rf test_speed768-90s
	-$(RM) -rf test_speed1024-90s
	-$(RM) -rf keccak4x/KeccakP-1600-times4-SIMD256.o
