CC 		    = /usr/bin/cc
CFLAGS    = -g -Wall -Wextra -O3 -fomit-frame-pointer -march=native -fPIC
NISTFLAGS = -O3 -fomit-frame-pointer -march=native -fPIC
CLANG     = clang -march=native -O3 -fomit-frame-pointer -fwrapv -Qunused-arguments
RM 		    = /bin/rm
#ifneq ($(shell uname -s),Darwin) # Add static if not on Darwin
#	CFLAGS += -static
#endif


all: test/test_kyber \
		 test/test_kex \
		 test/testvectors512 \
		 test/testvectors768 \
		 test/testvectors1024 \
     test/PQCgenKAT_kem \
		 test/speed

SOURCES = kem.c poly.c polyvec.c fips202.o reduce.c cbdeta4.s cbdref.c precomp.c ntt.c verify.c indcpa.c kex.c consts.c genmatrix.c fips202x4.c \
					keccak4x/KeccakP-1600-times4-SIMD256.o
HEADERS = params.h poly.h polyvec.h reduce.h cbd.h ntt.h verify.h indcpa.h kex.h genmatrix.h fips202x4.h

test/test_kyber: $(SOURCES) $(HEADERS) test/test_kyber.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kyber.c

test/PQCgenKAT_kem: $(SOURCES) $(HEADERS) test/PQCgenKAT_kem.c rng.c rng.h
	$(CC) $(NISTFLAGS) -o $@ $(SOURCES) -I. rng.c test/PQCgenKAT_kem.c -lcrypto

test/test_kex: $(SOURCES) $(HEADERS) test/test_kex.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kex.c

test/testvectors512: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=2 $(SOURCES) cpucycles.c test/testvectors.c

test/testvectors768: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=3 $(SOURCES) cpucycles.c test/testvectors.c

test/testvectors1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c randombytes.h
	$(CC) $(CFLAGS) -o $@ -DKYBER_K=4 $(SOURCES) cpucycles.c test/testvectors.c

test/speed: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/speed.c randombytes.c randombytes.h
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c cpucycles.c test/speed.c

keccak4x/KeccakP-1600-times4-SIMD256.o: keccak4x/KeccakP-1600-times4-SIMD256.c \
																				keccak4x/align.h \
																				keccak4x/brg_endian.h \
																				keccak4x/KeccakP-1600-times4-SIMD256.c \
																				keccak4x/KeccakP-1600-times4-SnP.h \
																				keccak4x/KeccakP-1600-unrolling.macros \
																				keccak4x/SIMD256-config.h
	$(CC) $(CFLAGS) -c keccak4x/KeccakP-1600-times4-SIMD256.c -o $@

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
	-$(RM) -r test/testvectors512
	-$(RM) -r test/testvectors768
	-$(RM) -r test/testvectors1024
	-$(RM) -r test/speed
	-$(RM) -r test/PQCgenKAT_kem
