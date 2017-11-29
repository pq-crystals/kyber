CC 		    = /usr/bin/cc
CFLAGS    = -g -Wall -Wextra -O3 -fomit-frame-pointer -march=native -fPIC
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -fPIC
RM 		    = /bin/rm

all: test_kyber512 \
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
     PQCgenKAT_kem \

SOURCES = kem.c poly.c polyvec.c fips202.c reduce.c cbd.c precomp.c ntt.c verify.c indcpa.c kex.c
HEADERS = params.h poly.h polyvec.h reduce.h cbd.h ntt.h verify.h indcpa.h kex.h

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

PQCgenKAT_kem: $(SOURCES) $(HEADERS) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCES) -I. rng.c PQCgenKAT_kem.c -lcrypto


.PHONY: clean test

clean:
	-$(RM) *.o
	-$(RM) -r test_kyber512
	-$(RM) -r test_kyber768
	-$(RM) -r test_kyber1024
	-$(RM) -r test_kex512
	-$(RM) -r test_kex768
	-$(RM) -r test_kex1024
	-$(RM) -r testvectors512
	-$(RM) -r testvectors768
	-$(RM) -r testvectors1024
	-$(RM) -r speed512
	-$(RM) -r speed768
	-$(RM) -r speed1024
	-$(RM) -r PQCgenKAT_kem
