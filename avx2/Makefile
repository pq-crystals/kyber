CC = /usr/bin/cc
CFLAGS += -Wall -Wextra -O3 -fomit-frame-pointer -march=native -mtune=native
#CFLAGS += -DUSE_RDPMC
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -mtune=native -fPIC
RM = /bin/rm

all: \
  test_kyber512 \
  test_kyber768 \
  test_kyber1024 \
  test_kex512 \
  test_kex768 \
  test_kex1024 \
  testvectors512 \
  testvectors768 \
  testvectors1024 \
  speed512 \
  speed768 \
  speed1024 \
  test_kyber512-90s \
  test_kyber768-90s \
  test_kyber1024-90s \
  test_kex512-90s \
  test_kex768-90s \
  test_kex1024-90s \
  testvectors512-90s \
  testvectors768-90s \
  testvectors1024-90s \
  speed512-90s \
  speed768-90s \
  speed1024-90s \
  PQCgenKAT_kem

SOURCES = kem.c poly.c polyvec.c fq.s shuffle.s cbd.c ntt.s invntt.s basemul.S consts.c verify.c indcpa.c rejsample.c
SOURCESKECCAK   = $(SOURCES) fips202.c fips202x4.c symmetric-fips202.c keccak4x/KeccakP-1600-times4-SIMD256.o
SOURCESNINETIES = $(SOURCES) aes256ctr.c
HEADERS = params.h poly.h polyvec.h reduce.h fq.inc cbd.h ntt.h shuffle.inc verify.h indcpa.h rejsample.h symmetric.h randombytes.h
HEADERSKECCAK   = $(HEADERS) fips202.h fips202x4.h
HEADERSNINETIES = $(HEADERS) aes256ctr.h

keccak4x/KeccakP-1600-times4-SIMD256.o: keccak4x/KeccakP-1600-times4-SIMD256.c \
  keccak4x/align.h keccak4x/brg_endian.h keccak4x/KeccakP-1600-times4-SnP.h \
  keccak4x/KeccakP-1600-unrolling.macros keccak4x/SIMD256-config.h
	$(CC) $(CFLAGS) -c $< -o $@

test_kyber512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber512

test_kyber768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber768

test_kyber1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber1024

test_kex512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex512

test_kex768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex768

test_kex1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex1024

testvectors512: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors512

testvectors768: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors768

testvectors1024: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors1024

speed512: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) kex.c randombytes.c cpucycles.c speed.c -o speed512

speed768: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) kex.c randombytes.c cpucycles.c speed.c -o speed768

speed1024: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) kex.c randombytes.c cpucycles.c speed.c -o speed1024


test_kyber512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber512-90s -lcrypto

test_kyber768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber768-90s -lcrypto

test_kyber1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber1024-90s -lcrypto

test_kex512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c randombytes.h fips202.h fips202.c kex.h kex.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex512-90s -lcrypto

test_kex768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c randombytes.h fips202.h fips202.c kex.h kex.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex768-90s -lcrypto

test_kex1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c randombytes.h fips202.h fips202.c kex.h kex.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex1024-90s -lcrypto

testvectors512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors512-90s -lcrypto

testvectors768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors768-90s -lcrypto

testvectors1024-90s: $(SOURCES) $(HEADERSNINETIES) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors1024-90s -lcrypto

speed512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c fips202.h fips202.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) kex.c fips202.c randombytes.c cpucycles.c speed.c -o speed512-90s -lcrypto

speed768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c fips202.h fips202.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) kex.c fips202.c randombytes.c cpucycles.c speed.c -o speed768-90s -lcrypto

speed1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h kex.h kex.c fips202.h fips202.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) kex.c fips202.c randombytes.c cpucycles.c speed.c -o speed1024-90s -l crypto


PQCgenKAT_kem: $(SOURCESKECCAK) $(HEADERSKECCAK) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCESKECCAK) -I. rng.c PQCgenKAT_kem.c -lcrypto

.PHONY: clean test

clean:
	-$(RM) -rf *.o
	-$(RM) -rf keccak4x/*.o
	-$(RM) -rf test_kyber512
	-$(RM) -rf test_kyber768
	-$(RM) -rf test_kyber1024
	-$(RM) -rf test_kex512
	-$(RM) -rf test_kex768
	-$(RM) -rf test_kex1024
	-$(RM) -rf testvectors512
	-$(RM) -rf testvectors768
	-$(RM) -rf testvectors1024
	-$(RM) -rf speed512
	-$(RM) -rf speed768
	-$(RM) -rf speed1024
	-$(RM) -rf PQCgenKAT_kem
	-$(RM) -rf test_kyber512-90s
	-$(RM) -rf test_kyber768-90s
	-$(RM) -rf test_kyber1024-90s
	-$(RM) -rf test_kex512-90s
	-$(RM) -rf test_kex768-90s
	-$(RM) -rf test_kex1024-90s
	-$(RM) -rf testvectors512-90s
	-$(RM) -rf testvectors768-90s
	-$(RM) -rf testvectors1024-90s
	-$(RM) -rf speed512-90s
	-$(RM) -rf speed768-90s
	-$(RM) -rf speed1024-90s
