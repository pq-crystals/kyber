CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -O3 -fomit-frame-pointer
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer
RM = /bin/rm

SOURCES = kex.c kem.c indcpa.c polyvec.c poly.c ntt.c cbd.c reduce.c verify.c
SOURCESKECCAK = $(SOURCES) fips202.c symmetric-shake.c
SOURCESNINETIES = $(SOURCES) sha256.c sha512.c aes256ctr.c symmetric-aes.c
HEADERS = params.h kex.h kem.h indcpa.h polyvec.h poly.h ntt.h cbd.h reduce.c verify.h symmetric.h
HEADERSKECCAK = $(HEADERS) fips202.h
HEADERSNINETIES = $(HEADERS) aes256ctr.h sha2.h

.PHONY: all speed shared clean

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
  test_vectors1024-90s

speed: \
  test_speed512 \
  test_speed768 \
  test_speed1024 \
  test_speed512-90s \
  test_speed768-90s \
  test_speed1024-90s

shared: \
  libpqcrystals_kyber512_ref.so \
  libpqcrystals_kyber768_ref.so \
  libpqcrystals_kyber1024_ref.so \
  libpqcrystals_kyber512-90s_ref.so \
  libpqcrystals_kyber768-90s_ref.so \
  libpqcrystals_kyber1024-90s_ref.so \
  libpqcrystals_fips202_ref.so \
  libpqcrystals_aes256ctr_ref.so \
  libpqcrystals_sha2_ref.so

libpqcrystals_fips202_ref.so: fips202.c fips202.h
	$(CC) -shared -fPIC $(CFLAGS) fips202.c -o libpqcrystals_fips202_ref.so

libpqcrystals_aes256ctr_ref.so: aes256ctr.c aes256ctr.h
	$(CC) -shared -fPIC $(CFLAGS) aes256ctr.c -o libpqcrystals_aes256ctr_ref.so

libpqcrystals_sha2_ref.so: sha256.c sha512.c sha2.h
	$(CC) -shared -fPIC $(CFLAGS) sha256.c sha512.c -o libpqcrystals_sha2_ref.so

libpqcrystals_kyber512_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=2 $(SOURCES) symmetric-shake.c -o libpqcrystals_kyber512_ref.so

libpqcrystals_kyber768_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=3 $(SOURCES) symmetric-shake.c -o libpqcrystals_kyber768_ref.so

libpqcrystals_kyber1024_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=4 $(SOURCES) symmetric-shake.c -o libpqcrystals_kyber1024_ref.so

test_kyber512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber512

test_kyber768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber768

test_kyber1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c test_kyber.c -o test_kyber1024

test_kex512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c test_kex.c -o test_kex512

test_kex768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c test_kex.c -o test_kex768

test_kex1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c test_kex.c -o test_kex1024

test_vectors512: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) test_vectors.c -o test_vectors512

test_vectors768: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) test_vectors.c -o test_vectors768

test_vectors1024: $(SOURCESKECCAK) $(HEADERSKECCAK) test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) test_vectors.c -o test_vectors1024

test_speed512: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESKECCAK) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed512

test_speed768: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESKECCAK) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed768

test_speed1024: $(SOURCESKECCAK) $(HEADERSKECCAK) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESKECCAK) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed1024

libpqcrystals_kyber512-90s_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=2 -DKYBER_90S $(SOURCES) symmetric-aes.c -o libpqcrystals_kyber512-90s_ref.so

libpqcrystals_kyber768-90s_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=3 -DKYBER_90S $(SOURCES) symmetric-aes.c -o libpqcrystals_kyber768-90s_ref.so

libpqcrystals_kyber1024-90s_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=4 -DKYBER_90S $(SOURCES) symmetric-aes.c -o libpqcrystals_kyber1024-90s_ref.so

test_kyber512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber512-90s

test_kyber768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber768-90s

test_kyber1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c test_kyber.c -o test_kyber1024-90s

test_kex512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c test_kex.c -o test_kex512-90s

test_kex768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c test_kex.c -o test_kex768-90s

test_kex1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_kex.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c test_kex.c -o test_kex1024-90s

test_vectors512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) test_vectors.c -o test_vectors512-90s

test_vectors768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) test_vectors.c -o test_vectors768-90s

test_vectors1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) test_vectors.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) test_vectors.c -o test_vectors1024-90s

test_speed512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=2 $(SOURCESNINETIES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed512-90s

test_speed768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=3 $(SOURCESNINETIES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed768-90s

test_speed1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) -D KYBER_90S -DKYBER_K=4 $(SOURCESNINETIES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed1024-90s

PQCgenKAT_kem512: $(SOURCESKECCAK) $(HEADERSKECCAK) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=2 -o $@ $(SOURCESKECCAK) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

PQCgenKAT_kem768: $(SOURCESKECCAK) $(HEADERSKECCAK) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=3 -o $@ $(SOURCESKECCAK) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

PQCgenKAT_kem1024: $(SOURCESKECCAK) $(HEADERSKECCAK) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=4 -o $@ $(SOURCESKECCAK) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

PQCgenKAT_kem512-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=2 -DKYBER_90S -o $@ $(SOURCESNINETIES) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

PQCgenKAT_kem768-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=3 -DKYBER_90S -o $@ $(SOURCESNINETIES) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

PQCgenKAT_kem1024-90s: $(SOURCESNINETIES) $(HEADERSNINETIES) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=4 -DKYBER_90S -o $@ $(SOURCESNINETIES) rng.c PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

clean:
	-$(RM) -rf *.gcno *.gcda *.lcov *.o *.so
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
	-$(RM) -rf PQCgenKAT_kem512
	-$(RM) -rf PQCgenKAT_kem768
	-$(RM) -rf PQCgenKAT_kem1024
	-$(RM) -rf PQCgenKAT_kem512-90s
	-$(RM) -rf PQCgenKAT_kem768-90s
	-$(RM) -rf PQCgenKAT_kem1024-90s
