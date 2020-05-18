#!/bin/sh -e

for dir in ref avx2; do
  make -C $dir
  for alg in 512 768 1024 512-90s 768-90s 1024-90s; do
    ./$dir/test_vectors$alg > tvecs$alg
  done
  sha256sum -c SHA256SUMS
done
