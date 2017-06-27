CC 		  = /usr/bin/cc
CFLAGS  = -g -Wall -Wextra -O3 -fomit-frame-pointer -march=native
CLANG   = clang -march=native -O3 -fomit-frame-pointer -fwrapv -Qunused-arguments
RM 		  = /bin/rm
ifneq ($(shell uname -s),Darwin) # Add static if not on Darwin
	CFLAGS += -static
endif


all: test/test_kyber \
		 test/test_kex \
		 test/testvectors \
		 test/speed

SOURCES = kyber.c poly.c polyvec.c fips202.o reduce.c cbd.c precomp.c ntt.c verify.c indcpa.c kex.c
HEADERS = params.h poly.h polyvec.h randombytes.h reduce.h cbd.h ntt.h verify.h indcpa.h kex.h

test/test_kyber: $(SOURCES) $(HEADERS) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kyber.c

test/test_kex: $(SOURCES) $(HEADERS) test/test_kex.c randombytes.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c test/test_kex.c

test/testvectors: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/testvectors.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) cpucycles.c test/testvectors.c

test/speed: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c test/speed.c randombytes.c
	$(CC) $(CFLAGS) -o $@ $(SOURCES) randombytes.c cpucycles.c test/speed.c

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
	-$(RM) -r test/test_kyber{,.dSYM}
	-$(RM) -r test/test_kex{,.dSYM}
	-$(RM) -r test/testvectors{,.dSYM}
	-$(RM) -r test/speed{,.dSYM}
