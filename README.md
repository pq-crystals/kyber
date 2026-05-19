# Kyber

# Custom Kyber Implementation for PQC Research & Benchmarking

This repository is a fork of the official [Kyber](https://github.com/pq-crystals/kyber) implementation, customized by **quannguyen247** for the purpose of experimenting with, modifying, and benchmarking Post-Quantum Cryptography (PQC) algorithms.

## Purpose
The primary goal of this fork is to provide a flexible framework for research and detailed performance analysis. Key objectives include:
- **Algorithmic Experimentation:** Facilitating modifications to core components such as hash functions and matrix operations to study their impact on security and efficiency.
- **Comparative Analysis:** Enabling side-by-side performance comparisons of different implementation strategies and optimizations.
- **Performance Metrics:** Precise execution time measurement for key generation, encapsulation, and decapsulation.

## Key Modifications
Significant changes have been made to the `ref` implementation to support these goals. For a detailed list of all modifications, please refer to the [CHANGELOG.md](CHANGELOG.md) file.

# Kyber
[![Build Status](https://travis-ci.org/pq-crystals/kyber.svg?branch=master)](https://travis-ci.org/pq-crystals/kyber) 
[![Coverage Status](https://coveralls.io/repos/github/pq-crystals/kyber/badge.svg?branch=master)](https://coveralls.io/github/pq-crystals/kyber?branch=master)

This repository contains the official reference implementation of the [Kyber](https://www.pq-crystals.org/kyber/) key encapsulation mechanism, 
and an optimized implementation for x86 CPUs supporting the AVX2 instruction set. 
Kyber has been selected for standardization in [round 3](https://csrc.nist.gov/Projects/post-quantum-cryptography/round-3-submissions) 
of the [NIST PQC](https://csrc.nist.gov/projects/post-quantum-cryptography) standardization project.


## Build instructions

The implementations contain several test and benchmarking programs and a Makefile to facilitate compilation.

### Prerequisites

Some of the test programs require [OpenSSL](https://openssl.org). 
If the OpenSSL header files and/or shared libraries do not lie in one of the standard locations on your system, 
it is necessary to specify their location via compiler and linker flags in the environment variables `CFLAGS`, `NISTFLAGS`, and `LDFLAGS`.

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

### Building all binaries

To compile the test and benchmarking programs on Linux or macOS, go to the `ref/` or `avx2/` directory and run
```sh
make
```
This produces the executables
```sh
test/test_kyber$ALG
test/test_vectors$ALG
test/test_speed$ALG
```
where `$ALG` ranges over the parameter sets 512, 768, 1024.

* `test_kyber$ALG` tests 1000 times to generate keys, encapsulate a random key and correctly decapsulate it again. 
  Also, the program tests that the keys cannot correctly be decapsulated using a random secret key 
  or a ciphertext where a single random byte was randomly distorted in order to test for trivial failures of the CCA security. 
  The program will abort with an error message and return 1 if there was an error. 
  Otherwise it will output the key and ciphertext sizes and return 0.
* `test_vectors$ALG` generates 10000 sets of test vectors containing keys, ciphertexts and shared secrets 
  whose byte-strings are output in hexadecimal. It also generates test vector for decapsulation of invalid
  (pseudorandom) ciphertexts.
  The required random bytes are deterministic and come from SHAKE128 on empty input.
* `test_speed$ALG` reports the median and average cycle counts of 1000 executions of various internal functions 
  and the API functions for key generation, encapsulation and decapsulation. 
  By default the Time Step Counter is used. 
  If instead you want to obtain the actual cycle counts from the Performance Measurement Counters, export `CFLAGS="-DUSE_RDPMC"` before compilation.

Please note that the reference implementation in `ref/` is not optimized for any platform, and, since it prioritises clean code, 
is significantly slower than a trivially optimized but still platform-independent implementation. 
Hence benchmarking the reference code does not provide particularly meaningful results.

<!--
Our Kyber implementations are contained in the [SUPERCOP](https://bench.cr.yp.to) benchmarking framework. 
See [here](http://bench.cr.yp.to/results-kem.html#amd64-kizomba) for cycle counts on an Intel KabyLake CPU.
-->

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

