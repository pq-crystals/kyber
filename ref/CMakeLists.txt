

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wredundant-decls")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -z noexecstack")
set(NISTFLAGS "${NISTFLAGS}" -Wno-unused-result -O3 -fomit-frame-pointer)

set(securitylevel 2 3 4)

# build randombytes
add_library(randombytes SHARED randombytes.c)

# build nist rng
add_library(rng SHARED nistkat/rng.c)
target_link_libraries(rng OpenSSL::Crypto)

set(SRCS
  kem.c
  indcpa.c
  polyvec.c
  poly.c
  ntt.c
  cbd.c
  reduce.c
  verify.c
)

# build fips202 shared library
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)
  set(name pqccrystals_fips202_${NBITS}_ref)

  add_library(${name} SHARED fips202.c symmetric-shake.c)

  target_compile_definitions(${name} PUBLIC KYBER_K=${level})
endforeach()

# build algorithm shared library
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)
  set(name pqccrystals_kyber${NBITS}_ref)

  add_library(${name} SHARED ${SRCS})
  target_compile_definitions(${name} PUBLIC KYBER_K=${level})
endforeach()

# build test_kyber
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_kyber${NBITS} test/test_kyber.c)
  target_link_libraries(test_kyber${NBITS}
    pqccrystals_fips202_${NBITS}_ref
    pqccrystals_kyber${NBITS}_ref
    randombytes)
endforeach()

# build test_vector
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_vectors${NBITS} test/test_vectors.c)
  target_link_libraries(test_vectors${NBITS}
    pqccrystals_fips202_${NBITS}_ref
    pqccrystals_kyber${NBITS}_ref)
endforeach()

# build test_speed
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_executable(test_speed${NBITS}
    test/cpucycles.c
    test/speed_print.c
    test/test_speed.c
	)
  target_link_libraries(test_speed${NBITS}
    pqccrystals_fips202_${NBITS}_ref
    pqccrystals_kyber${NBITS}_ref
    randombytes)
endforeach()

# build nistkat
foreach(level IN LISTS securitylevel)
  MATH(EXPR NBITS "256 * ${level}" OUTPUT_FORMAT DECIMAL)

  add_compile_options(${NISTFLAGS})
  add_executable(PQCgenKAT_kem${NBITS} nistkat/PQCgenKAT_kem.c)
  target_link_libraries(PQCgenKAT_kem${NBITS}
    pqccrystals_fips202_${NBITS}_ref
    pqccrystals_kyber${NBITS}_ref
    rng)
endforeach()