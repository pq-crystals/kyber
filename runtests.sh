#!/bin/sh -e

ARCH="${ARCH:-amd64}"
ARCH="${TRAVIS_CPU_ARCH:-$ARCH}"

if [ "$ARCH" = "amd64" -a "$TRAVIS_OS_NAME" != "osx" ]; then
  DIRS="ref avx2"
else
  DIRS="ref"
fi

for dir in $DIRS; do
  make -C $dir
  for alg in 512 768 1024 512-90s 768-90s 1024-90s; do
    #valgrind --vex-guest-max-insns=25 ./$dir/test_kyber$alg 2>&1 | grep ERROR
    ./$dir/test_kyber$alg
    ./$dir/test_vectors$alg > tvecs$alg
  done
  shasum -a256 -c SHA256SUMS
done

exit 0
