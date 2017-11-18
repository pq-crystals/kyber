CC 		    = /usr/bin/cc
CFLAGS    = -g -Wall -Wextra -O3 -fomit-frame-pointer -march=native -fPIC
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -fPIC
CLANG     = clang -march=native -O3 -fomit-frame-pointer -fwrapv -Qunused-arguments
RM 		    = /bin/rm

all: test_kyber \
		 test_kex \
		 testvectors512 \
		 testvectors768 \
		 testvectors1024 \
		 speed512 \
		 speed768 \
		 speed1024 \
     PQCgenKAT_kem \

SOURCES = kem.c poly.c polyvec.c fips202.o reduce.c cbd.c precomp.c ntt.c verify.c indcpa.c kex.c
HEADERS = params.h poly.h polyvec.h reduce.h cbd.h ntt.h verify.h indcpa.h kex.h

test_kyber: $(SOURCES) $(HEADERS) test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test_kyber.c

PQCgenKAT_kem: $(SOURCES) $(HEADERS) PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCES) -I. rng.c PQCgenKAT_kem.c -lcrypto

test_kex: $(SOURCES) $(HEADERS) test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test_kex.c

testvectors512: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=2 $(SOURCES) cpucycles.c testvectors.c

testvectors768: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=3 $(SOURCES) cpucycles.c testvectors.c

testvectors1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=4 $(SOURCES) cpucycles.c testvectors.c

speed512: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=2 $(SOURCES) randombytes.c cpucycles.c speed.c

speed768: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=3 $(SOURCES) randombytes.c cpucycles.c speed.c

speed1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=4 $(SOURCES) randombytes.c cpucycles.c speed.c


fips202.o: fips202.c
	$(CLANG) -c -fPIC $^ -o $@

.PHONY: clean test

clean:
	-$(RM) *.o
	-$(RM) -r test_kyber
	-$(RM) -r test_kex
	-$(RM) -r testvectors512
	-$(RM) -r testvectors768
	-$(RM) -r testvectors1024
	-$(RM) -r speed512
	-$(RM) -r speed768
	-$(RM) -r speed1024
	-$(RM) -r PQCgenKAT_kem
