#ifndef __EVILIBC_LIMITS_H
#define __EVILIBC_LIMITS_H

/*
 * Compiler-dependent values. Nothing interesting here unless we're using
 * an evil compiler (TODO).
 */

/* TODO: assumes x86_64 */

#define CHAR_BIT 8

#define SCHAR_MAX 127
#define SCHAR_MIN (-SCHAR_MAX - 1)
#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN 0
#define CHAR_MAX UCHAR_MAX
#else
#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX
#endif

#define MB_LEN_MAX 16

#define SHRT_MAX 32767
#define SHRT_MIN (-SHRT_MAX - 1)
#define USHRT_MAX 65535

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 4294967295

#define LONG_MAX 9223372036854775807L
#define LONG_MIN (-LONG_MAX - 1LL)
#define ULONG_MAX 18446744073709551615UL

#define LLONG_MAX	9223372036854775807LL
#define LLONG_MIN	(-LLONG_MAX - 1LL)
#define ULLONG_MAX	18446744073709551615ULL

#endif /* __EVILIBC_LIMITS_H */
