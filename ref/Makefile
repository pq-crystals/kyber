CC 		    = /usr/bin/cc
CFLAGS    = -g -Wall -Wextra -O3 -fomit-frame-pointer -march=native -fPIC
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -fPIC
CLANG     = clang -march=native -O3 -fomit-frame-pointer -fwrapv -Qunused-arguments
RM 		    = /bin/rm

all: test/test_kyber \
		 test/test_kex \
		 test/testvectors \
     test/PQCgenKAT_kem \
		 test/speed

SOURCES = kem.c poly.c polyvec.c fips202.o reduce.c cbd.c precomp.c ntt.c verify.c indcpa.c kex.c
HEADERS = params.h poly.h polyvec.h reduce.h cbd.h ntt.h verify.h indcpa.h kex.h

test/test_kyber: $(SOURCES) $(HEADERS) test/test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kyber.c

test/PQCgenKAT_kem: $(SOURCES) $(HEADERS) test/PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCES) -I. rng.c test/PQCgenKAT_kem.c -lcrypto

test/test_kex: $(SOURCES) $(HEADERS) test/test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kex.c

test/testvectors: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) cpucycles.c test/testvectors.c

test/speed: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c cpucycles.c test/speed.c

fips202.o: fips202.c
	$(CLANG) -c -fPIC $^ -o $@

.PHONY: clean test

test: 
	./test/test_kex
	./test/test_kyber
	./test/testvectors
	./test/speed

clean:
	-$(RM) *.o
	-$(RM) -r test/test_kyber
	-$(RM) -r test/test_kex
	-$(RM) -r test/testvectors
	-$(RM) -r test/speed
	-$(RM) -r test/PQCgenKAT_kem
