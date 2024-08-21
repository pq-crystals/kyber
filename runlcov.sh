#!/bin/sh -e

cd ref

for alg in 512 768 1024; do
  make -B test/test_kyber$alg CFLAGS="-O0 -g --coverage"
  ./test/test_kyber$alg
  lcov -c -d . -o kyber$alg.lcov
  lcov -z -d .
  rm test/test_kyber$alg
done

lcov -o kyber.lcov \
  -a kyber512.lcov \
  -a kyber768.lcov \
  -a kyber1024.lcov \

lcov -r kyber.lcov -o kyber.lcov \
  '*/test/test_kyber.c'

exit 0
