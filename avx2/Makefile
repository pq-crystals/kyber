CC 		  = /usr/bin/gcc
CFLAGS  = -Wall -Wextra -O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native
CLANG   = clang -march=native -O3 -fomit-frame-pointer -fwrapv -Qunused-arguments
RM 		  = /bin/rm
ifneq ($(shell uname -s),Darwin) # Add static if not on Darwin
	CFLAGS += -static
endif

all: test/test_kyber \
		 test/test_kex \
		 test/testvectors \
		 test/speed

SOURCES = kyber.c poly.c polyvec.c fips202.o fips202x4.c precomp.c ntt.c verify.c indcpa.c consts.c kex.c \
				  nttlevels0t4.s nttlevels5t7.s mul_coefficients.s polyvec_pointwise_acc.s poly_add.s poly_sub.s cbd.s poly_freeze.s \
					crypto_stream_aes256ctr.c \
					keccak4x/KeccakP-1600-times4-SIMD256.o

HEADERS = params.h poly.h polyvec.h randombytes.h cbd.h ntt.h verify.h indcpa.h fips202.h fips202x4.h kex.h

test/test_kyber: $(SOURCES) $(HEADERS) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kyber.c

test/test_kex: $(SOURCES) $(HEADERS) test/test_kex.c randombytes.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kex.c

test/testvectors: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c
	$(CC) $(CFLAGS) -DTESTVECTORS -o $@ $(SOURCES) cpucycles.c test/testvectors.c 

test/speed: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/speed.c randombytes.c
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
	$(CLANG) -c $^ -o $@


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
	-$(RM) -r keccak4x/KeccakP-1600-times4-SIMD256.o

