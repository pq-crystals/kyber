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
  PQCgenKAT_kem \

SOURCES = kem.c poly.c polyvec.c fq.s shuffle.s cbd.c ntt.s invntt.s basemul.S consts.c verify.c indcpa.c kex.c rejsample.c aes256ctr.c fips202.c
HEADERS = params.h poly.h polyvec.h reduce.h fq.h cbd.h ntt.h shuffle.h verify.h indcpa.h kex.h rejsample.h symmetric.h aes256ctr.h randombytes.h

test_kyber512: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCES) randombytes.c test_kyber.c -o test_kyber512

test_kyber768: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCES) randombytes.c test_kyber.c -o test_kyber768

test_kyber1024: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCES) randombytes.c test_kyber.c -o test_kyber1024

test_kex512: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCES) randombytes.c test_kex.c -o test_kex512

test_kex768: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCES) randombytes.c test_kex.c -o test_kex768

test_kex1024: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCES) randombytes.c test_kex.c -o test_kex1024

testvectors512: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCES) cpucycles.c testvectors.c -o testvectors512

testvectors768: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCES) cpucycles.c testvectors.c -o testvectors768

testvectors1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCES) cpucycles.c testvectors.c -o testvectors1024

speed512: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed512

speed768: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed768

speed1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed1024


test_kyber512-90s: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCES) randombytes.c test_kyber.c -o test_kyber512-90s -lcrypto

test_kyber768-90s: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCES) randombytes.c test_kyber.c -o test_kyber768-90s -lcrypto

test_kyber1024-90s: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCES) randombytes.c test_kyber.c -o test_kyber1024-90s -lcrypto

test_kex512-90s: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCES) randombytes.c test_kex.c -o test_kex512-90s -lcrypto

test_kex768-90s: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCES) randombytes.c test_kex.c -o test_kex768-90s -lcrypto

test_kex1024-90s: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCES) randombytes.c test_kex.c -o test_kex1024-90s -lcrypto

testvectors512-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCES) cpucycles.c testvectors.c -o testvectors512-90s -lcrypto

testvectors768-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCES) cpucycles.c testvectors.c -o testvectors768-90s -lcrypto

testvectors1024-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCES) cpucycles.c testvectors.c -o testvectors1024-90s -lcrypto

speed512-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed512-90s -lcrypto

speed768-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCES) fastrandombytes.c kernelrandombytes.c cpucycles.c speed.c -o speed768-90s -lcrypto

speed1024-90s: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCES) randombytes.c cpucycles.c speed.c -o speed1024-90s


PQCgenKAT_kem: $(SOURCES) $(HEADERS) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCES) -I. rng.c PQCgenKAT_kem.c -lcrypto

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
	-$(RM) -rf speed512
	-$(RM) -rf speed768
	-$(RM) -rf speed1024
	-$(RM) -rf PQCgenKAT_kem
