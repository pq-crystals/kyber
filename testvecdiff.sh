#!/bin/bash

cd avx2 && make
cd ../ref && make
cd ..

./ref/test_vectors512 > ref/t512
./ref/test_vectors768 > ref/t768
./ref/test_vectors1024 > ref/t1024
./ref/test_vectors512-90s > ref/t512-90s
./ref/test_vectors768-90s > ref/t768-90s
./ref/test_vectors1024-90s > ref/t1024-90s
./avx2/test_vectors512 > avx2/t512
./avx2/test_vectors768 > avx2/t768
./avx2/test_vectors1024 > avx2/t1024
./avx2/test_vectors512-90s > avx2/t512-90s
./avx2/test_vectors768-90s > avx2/t768-90s
./avx2/test_vectors1024-90s > avx2/t1024-90s

diff ref/t512 avx2/t512
diff ref/t768 avx2/t768
diff ref/t1024 avx2/t1024
diff ref/t512-90s avx2/t512-90s
diff ref/t768-90s avx2/t768-90s
diff ref/t1024-90s avx2/t1024-90s

diff t512 avx2/t512
diff t768 avx2/t768
diff t1024 avx2/t1024
diff t512-90s avx2/t512-90s
diff t768-90s avx2/t768-90s
diff t1024-90s avx2/t1024-90s

rm ref/t512
rm avx2/t512
rm ref/t768
rm avx2/t768
rm ref/t1024
rm avx2/t1024
rm ref/t512-90s
rm avx2/t512-90s
rm ref/t768-90s
rm avx2/t768-90s
rm ref/t1024-90s
rm avx2/t1024-90s
