
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wredundant-decls")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mbmi2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mpopcnt")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -z noexecstack")

set(NISTFLAGS "${NISTFLAGS}"
  -Wno-unused-result
  -mavx2
  -mbmi2
  -mpopcnt
  -march=native
  -mtune=native
  -O3
  -fomit-frame-pointer
)

set(securitylevel 2 3 4)


# build fips202 shared library
set(SRCS fips202.c fips202x4.c keccak4x/KeccakP-1600-times4-SIMD256.c symmetric-shake.c)

foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)
  set(name pqccrystals_fips202x4_avx2_${NBITS})

  add_library(${name} SHARED ${SRCS})
  target_compile_definitions(${name} PUBLIC KYBER_K=${level})
endforeach()

set(SRCS
  kem.c
  indcpa.c
  polyvec.c
  poly.c
  shuffle.S
  fq.S
  ntt.S
  invntt.S
  basemul.S
  consts.c
  rejsample.c
  cbd.c
  verify.c
)

# build algorithm shared library
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)
  set(name pqccrystals_kyber${NBITS}_avx2)

  add_library(${name} SHARED ${SRCS})
  target_compile_definitions(${name} PUBLIC KYBER_K=${level})
  target_link_libraries(${name} pqccrystals_fips202x4_avx2_${NBITS})
  target_include_directories(${name} PRIVATE .)
endforeach()

# build test_kyber
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_kyber${NBITS}_avx2 test/test_kyber.c)
  target_link_libraries(test_kyber${NBITS}_avx2
    pqccrystals_kyber${NBITS}_avx2
	  randombytes)
endforeach()

# build test_vector
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_vectors${NBITS}_avx2 test/test_vectors.c)
  target_link_libraries(test_vectors${NBITS}_avx2
    pqccrystals_kyber${NBITS}_avx2)
endforeach()

# build test_speed
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_speed${NBITS}_avx2
    test/cpucycles.c
	  test/speed_print.c
	  test/test_speed.c
	)
  target_link_libraries(test_speed${NBITS}_avx2
    pqccrystals_kyber${NBITS}_avx2
    randombytes)
endforeach()