CC = /usr/bin/cc
CFLAGS += -Wall -Wextra -O3 -fomit-frame-pointer -march=native -mtune=native -fPIC
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
  test_kyber512-90s \
  test_kyber768-90s \
  test_kyber1024-90s \
  test_kex512-90s \
  test_kex768-90s \
  test_kex1024-90s \
  testvectors512-90s \
  testvectors768-90s \
  testvectors1024-90s

SOURCES = kem.c poly.c polyvec.c reduce.c cbd.c ntt.c verify.c indcpa.c
SOURCESKECCAK = $(SOURCES) fips202.c symmetric-fips202.c
SOURCESNINETIES = $(SOURCES) sha256.c sha512.c aes256ctr.c
HEADERS = params.h poly.h polyvec.h reduce.h cbd.h ntt.h verify.h indcpa.h
HEADERSKECCAK = $(HEADERS) fips202.h
HEADERSNINETIES = $(HEADERS) aes256ctr.h sha2.h

test_kyber512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber512

test_kyber768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber768

test_kyber1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber1024

test_kex512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex512

test_kex768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex768

test_kex1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c randombytes.h kex.c kex.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c kex.c test_kex.c -o test_kex1024

testvectors512: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors512

testvectors768: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors768

testvectors1024: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) cpucycles.c testvectors.c -o testvectors1024

test_kyber512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber512-90s

test_kyber768-90s: $(SOURCESNINETIES) $(HEADERS90S) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber768-90s

test_kyber1024-90s: $(SOURCESNINETIES) $(HEADERS90S) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber1024-90s

test_kex512-90s: $(SOURCESNINETIES) $(HEADERS90S) test_kex.c randombytes.c randombytes.h fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex512-90s

test_kex768-90s: $(SOURCESNINETIES) $(HEADERS90S) test_kex.c randombytes.c randombytes.h fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex768-90s

test_kex1024-90s: $(SOURCESNINETIES) $(HEADERS90S) test_kex.c randombytes.c randombytes.h fips202.c fips202.h kex.c kex.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c fips202.c kex.c test_kex.c -o test_kex1024-90s

testvectors512-90s: $(SOURCESNINETIES) $(HEADERS90S) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors512-90s

testvectors768-90s: $(SOURCESNINETIES) $(HEADERS90S) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors768-90s

testvectors1024-90s: $(SOURCESNINETIES) $(HEADERS90S) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) cpucycles.c testvectors.c -o testvectors1024-90s

PQCgenKAT_kem: $(SOURCESKECCAK) $(HEADERSKECCAK) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCESKECCAK) -I. rng.c PQCgenKAT_kem.c -lcrypto

.PHONY: clean test

clean:
	-$(RM) -rf *.o
	-$(RM) -rf test_kyber512
	-$(RM) -rf test_kyber768
	-$(RM) -rf test_kyber1024
	-$(RM) -rf test_kex512
	-$(RM) -rf test_kex768
	-$(RM) -rf test_kex1024
	-$(RM) -rf testvectors512
	-$(RM) -rf testvectors768
	-$(RM) -rf testvectors1024
	-$(RM) -rf test_kyber512-90s
	-$(RM) -rf test_kyber768-90s
	-$(RM) -rf test_kyber1024-90s
	-$(RM) -rf test_kex512-90s
	-$(RM) -rf test_kex768-90s
	-$(RM) -rf test_kex1024-90s
	-$(RM) -rf testvectors512-90s
	-$(RM) -rf testvectors768-90s
	-$(RM) -rf testvectors1024-90s
	-$(RM) -rf PQCgenKAT_kem
