# Kyber

[![Build Status](https://travis-ci.org/pq-crystals/kyber.svg?branch=master)](https://travis-ci.org/pq-crystals/kyber) [![Coverage Status](https://coveralls.io/repos/github/pq-crystals/kyber/badge.svg?branch=master)](https://coveralls.io/github/pq-crystals/kyber?branch=master)

This repository contains the official reference implementation of the [Kyber](https://www.pq-crystals.org/kyber/) key encapsulation mechanism, and an optimized implementation for x86 CPUs supporting the AVX2 instruction set. Kyber is a finalist in [round 3](https://csrc.nist.gov/Projects/post-quantum-cryptography/round-3-submissions) of the [NIST PQC](https://csrc.nist.gov/projects/post-quantum-cryptography) standardization project.

## Build instructions

The implementations contain several test and benchmarking programs and a Makefile to facilitate compilation.

### Prerequisites

Some of the test programs require [OpenSSL](https://openssl.org). If the OpenSSL header files and/or shared libraries do not lie in one of the standard locations on your system, it is necessary to specify their location via compiler and linker flags in the environment variables `CFLAGS`, `NISTFLAGS`, and `LDFLAGS`.

For example, on macOS you can install OpenSSL via [Homebrew](https://brew.sh) by running
```sh
brew install openssl
```
Then, run
```sh
export CFLAGS="-I/usr/local/opt/openssl@1.1/include"
export NISTFLAGS="-I/usr/local/opt/openssl@1.1/include"
export LDFLAGS="-L/usr/local/opt/openssl@1.1/lib"
```
before compilation to add the OpenSSL header and library locations to the respective search paths.

### Test programs

To compile the test programs on Linux or macOS, go to the `ref/` or `avx2/` directory and run
```sh
make
```
This produces the executables
```sh
test/test_kyber$ALG
test/test_kex$ALG
test/test_vectors$ALG
```
where `$ALG` ranges over the parameter sets 512, 768, 1024, 512-90s, 768-90s, 1024-90s.

* `test_kyber$ALG` tests 1000 times to generate keys, encapsulate a random key and correctly decapsulate it again. Also, the program tests that the keys cannot correctly be decapsulated using a random secret key or a ciphertext where a single random byte was randomly distorted in order to test for trivial failures of the CCA security. The program will abort with an error message and return 1 if there was an error. Otherwise it will output the key and ciphertext sizes and return 0.
* `test_kex$ALG` tests the authenticated key exchange schemes derived from the Kyber KEM
* `test_vectors$ALG` generates 10000 sets of test vectors containing keys, ciphertexts and shared secrets whose byte-strings are output in hexadecimal. The required random bytes come from a simple deterministic expansion of a fixed seed defined in `test_vectors.c`.

### Benchmarking programs

For benchmarking the implementations, we provide speed test programs for x86 CPUs that use the Time Step Counter (TSC) or the actual cycle counter provided by the Performance Measurement Counters (PMC) to measure performance. To compile the programs run
```sh
make speed
```
This produces the executables
```sh
test/test_speed$ALG
```
for all parameter sets `$ALG` as above. The programs report the median and average cycle counts of 1000 executions of various internal functions and the API functions for key generation, encapsulation and decapsulation. By default the Time Step Counter is used. If instead you want to obtain the actual cycle counts from the Performance Measurement Counters, export `CFLAGS="-DUSE_RDPMC"` before compilation.

Please note that the reference implementation in `ref/` is not optimized for any platform, and, since it prioritises clean code, is significantly slower than a trivially optimized but still platform-independent implementation. Hence benchmarking the reference code does not provide meaningful results.

Our Kyber implementations are contained in the [SUPERCOP](https://bench.cr.yp.to) benchmarking framework. See [here](http://bench.cr.yp.to/results-kem.html#amd64-kizomba) for cycle counts on an Intel KabyLake CPU.

## Shared libraries

All implementations can be compiled into shared libraries by running
```sh
make shared
```
For example in the directory `ref/` of the reference implementation, this produces the libraries
```sh
libpqcrystals_kyber$ALG_ref.so
```
for all parameter sets `$ALG`, and the required symmetric crypto libraries
```
libpqcrystals_aes256ctr_ref.so
libpqcrystals_fips202_ref.so
```
All global symbols in the libraries lie in the namespaces `pqcrystals_kyber$ALG_ref`, `libpqcrystals_aes256ctr_ref` and `libpqcrystals_fips202_ref`. Hence it is possible to link a program against all libraries simultaneously and obtain access to all implementations for all parameter sets. The corresponding API header file is `ref/api.h`, which contains prototypes for all API functions and preprocessor defines for the key and signature lengths.

## CMake

Also available is a portable [cmake](https://cmake.org) based build system that permits building the reference implementation.

By calling 
```
mkdir build && cd build && cmake .. && cmake --build . && ctest
```

the Kyber reference implementation gets built and tested.
