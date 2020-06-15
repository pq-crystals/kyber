#!/bin/sh -e

cd ref

for alg in 512 512-90s 768 768-90s 1024 1024-90s; do
  make -B test_kyber$alg CFLAGS="-O0 -g --coverage"
  ./test_kyber$alg
  lcov -c -d . -o kyber$alg.lcov
  lcov -z -d .
  rm test_kyber$alg
done

lcov -o kyber.lcov \
  -a kyber512.lcov \
  -a kyber512-90s.lcov \
  -a kyber768.lcov \
  -a kyber768-90s.lcov \
  -a kyber1024.lcov \
  -a kyber1024-90s.lcov \

lcov -r kyber.lcov -o kyber.lcov \
  '*/test_kyber.c'

exit 0
