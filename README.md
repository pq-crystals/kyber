# Kyber

[![Build Status](https://travis-ci.org/pq-crystals/kyber.svg?branch=master)](https://travis-ci.org/pq-crystals/kyber) [![Coverage Status](https://coveralls.io/repos/github/pq-crystals/kyber/badge.svg?branch=master)](https://coveralls.io/github/pq-crystals/kyber?branch=master)

This directory contains our implementation of [Kyber](https://eprint.iacr.org/2017/634). Both the reference code and the AVX2 optimized code are in the directories ref/ and avx2/, respectively.

## CMake

Also available is a highly portable [cmake](https://cmake.org) based build system that permits building the same sources into a summary library as well as all the same tests.

For fastest build performance, use of [Ninja](https://ninja-build.org) is recommended.

All tests can be run by invoking the (your-favourite-build-tool-command-here) `test` target.

### Worked example

By calling 
```
mkdir build-ninja && cd build-ninja && cmake -DBUILD_SHARED_LIBS=ON -GNinja .. && ninja && ninja test
```

the whole Kyber software family gets built in a highly portable as well as an avx2-optimized version, tested and delivered in a shared library.

For example, by running `./avx2/./avx2/test_speed512-90s_avx2` in the newly created 'build-ninja' folder, performance testing of `Kyber512-90s` in the optimized AVX2 variant is executed.

The resultant library might also be installed using the `install` target.

