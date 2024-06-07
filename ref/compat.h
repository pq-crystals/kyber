#ifndef COMPAT_H
#define COMPAT_H

#ifdef _MSC_VER
#define KYBER_NOINLINE __declspec(noinline)
#else
#define KYBER_NOINLINE __attribute__((noinline))
#endif

#endif
