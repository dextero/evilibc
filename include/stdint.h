#ifndef __EVILIBC_STDINT_H
#define __EVILIBC_STDINT_H

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.20.1.1 Exact-width integer types:
 * > These types are optional. However, if an implementation provides integer
 * > types with widths of 8, 16, 32, or 64 bits, no padding bits, and (for the
 * > signed types) that have a two’s complement representation, it shall define
 * > the corresponding typedef names.
 *
 * POSIX requires exact-width types up to 32-bit ones to be present though.
 *
 * TODO: only fixed-width types are required to be two's complement. Would
 * it be technically correct to have different types have different encodings?
 * I.e. sign-magitude for for int_leastN_y and U2 for int_fastN_t?
 * TODO: some stupid 9-bit-based architecture and a compiler that does not
 * have these.
 * NOTE: that would no longer be POSIX; it requires CHAR_BIT == 8
 */
#ifdef _POSIX_VERSION

typedef __INT8_TYPE__  int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;

#define INT8_MIN __INT8_MIN__
#define INT8_MAX __INT8_MAX__
#define INT16_MIN __INT16_MIN__
#define INT16_MAX __INT16_MAX__
#define INT32_MIN __INT32_MIN__
#define INT32_MAX __INT32_MAX__

typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;

#define UINT8_MAX __UINT8_MAX__
#define UINT16_MAX __UINT16_MAX__
#define UINT32_MAX __UINT32_MAX__

#endif /* _POSIX_VERSION */

/*
 * These should be smallest possible types at least N bytes large, but
 * C spec allows them to be bigger.
 *
 * TODO: make 64-bit ones slightly bigger than required
 */
typedef __INT16_TYPE__  int_least8_t;
typedef __INT32_TYPE__  int_least16_t;
typedef __INT64_TYPE__  int_least32_t;
typedef __int128_t      int_least64_t;

#define INT8_LEAST_MIN  __INT_LEAST16_MIN__
#define INT8_LEAST_MAX  __INT_LEAST16_MAX__
#define INT16_LEAST_MIN __INT_LEAST32_MIN__
#define INT16_LEAST_MAX __INT_LEAST32_MAX__
#define INT32_LEAST_MIN __INT_LEAST64_MIN__
#define INT32_LEAST_MAX __INT_LEAST64_MAX__

/* FIXME: how to express 128-bit constants? */
#define INT64_LEAST_MIN __INT_LEAST64_MIN__
#define INT64_LEAST_MAX __INT_LEAST64_MAX__

/* Yes, these macros correspond to least-size types, not exact-size ones. */
#define INT8_C(v) v
#define INT16_C(v) v
#define INT32_C(v) v
#define INT64_C(v) v ## LL

typedef __UINT16_TYPE__ uint_least8_t;
typedef __UINT32_TYPE__ uint_least16_t;
typedef __UINT64_TYPE__ uint_least32_t;
typedef __uint128_t     uint_least64_t;

#define UINT8_LEAST_MIN  __UINT_LEAST16_MIN__
#define UINT8_LEAST_MAX  __UINT_LEAST16_MAX__
#define UINT16_LEAST_MIN __UINT_LEAST32_MIN__
#define UINT16_LEAST_MAX __UINT_LEAST32_MAX__
#define UINT32_LEAST_MIN __UINT_LEAST64_MIN__
#define UINT32_LEAST_MAX __UINT_LEAST64_MAX__

/* FIXME: how to express 128-bit constants? */
#define UINT64_LEAST_MAX __UINT_LEAST64_MAX__

#define UINT8_C(v) v ## U
#define UINT16_C(v) v ## U
#define UINT32_C(v) v ## U
#define UINT64_C(v) v ## ULL

typedef int_least8_t  int_fast8_t;
typedef int_least16_t int_fast16_t;
typedef int_least32_t int_fast32_t;
typedef int_least64_t int_fast64_t;

#define INT_FAST8_MIN  INT_LEAST8_MIN
#define INT_FAST8_MAX  INT_LEAST8_MAX
#define INT_FAST16_MIN INT_LEAST16_MIN
#define INT_FAST16_MAX INT_LEAST16_MAX
#define INT_FAST32_MIN INT_LEAST32_MIN
#define INT_FAST32_MAX INT_LEAST32_MAX
#define INT_FAST64_MIN INT_LEAST64_MIN
#define INT_FAST64_MAX INT_LEAST64_MAX

typedef uint_least8_t  uint_fast8_t;
typedef uint_least16_t uint_fast16_t;
typedef uint_least32_t uint_fast32_t;
typedef uint_least64_t uint_fast64_t;

#define UINT_FAST8_MAX  UINT_LEAST8_MAX
#define UINT_FAST16_MAX UINT_LEAST16_MAX
#define UINT_FAST32_MAX UINT_LEAST32_MAX
#define UINT_FAST64_MAX UINT_LEAST64_MAX

/*
 * 7.20.1.4:
 * > The following type designates a signed integer type with the property
 * > that any valid pointer to void can be converted to this type, then
 * > converted back to pointer to void, and the result will compare equal
 * > to the original pointer: intptr_t/uintptr_t are optional in the C spec.
 * >    intptr_t
 * > The following type designates an unsigned integer type with the property
 * > that any valid pointer to void can be converted to this type, then
 * > converted back to pointer to void, and the result will compare equal
 * > to the original pointer:
 * >    uintptr_t
 * > These types are optional.
 *
 * TODO: "any **valid** pointer"; is NULL considered a **valid** pointer?
 *
 * POSIX requires them to be present only on XSI-compliant systems
 *
 * http://pubs.opengroup.org/onlinepubs/007904975/functions/xsh_chap02_02.html :
 * > An XSI-conforming application should ensure that the feature test macro
 * > _XOPEN_SOURCE is defined with the value 600 before inclusion of any header.
 */
#if _XOPEN_SOURCE == 600

typedef __INTPTR_TYPE__  intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;

/*
 * TODO: according to 7.20.2.4, these values are allowed to be 16-bit.
 * It would probably require a custom compiler though.
 */
#define INTPTR_MIN  __INTPTR_MIN__
#define INTPTR_MAX  __INTPTR_MAX__
#define UINTPTR_MAX __UINTPTR_MAX__

#endif /* _XOPEN_SOURCE */

typedef __INTMAX_TYPE__  intmax_t;
typedef __UINTMAX_TYPE__ uintmax_t;

#define INTMAX_C(v) v ## LL
#define UINTMAX_C(v) v ## ULL

#define INTMAX_MAX  __INTMAX_MAX__
#define UINTMAX_MAX __UINTMAX_MAX__

#define PTRDIFF_MIN __PTRDIFF_MIN__
#define PTRDIFF_MAX __PTRDIFF_MAX__

#define SIG_ATOMIC_MIN CHAR_MIN
#define SIG_ATOMIC_MAX CHAR_MAX

#define SIZE_MAX __SIZE_MAX__

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDINT_H */
