#ifndef ___INTTYPES_H
#define ___INTTYPES_H

/*
 * 7.8, 1:
 * > The header <inttypes.h> includes the header <stdint.h> and extends it
 * > with additional facilities provided by hosted implementations.
 */
#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.8, 2:
 * > For each type declared in <stdint.h>, it defines corresponding macros
 * > for conversion specifiers for use with the formatted input/output
 * > functions.
 */

#if INT_MIN <= INT_LEAST8_MIN && INT_LEAST8_MAX <= INT_MAX
# define __FMTdLEAST8 "d"
#elif LONG_MIN <= INT_LEAST8_MIN && INT_LEAST8_MAX <= LONG_MAX
# define __FMTdLEAST8 "ld"
#elif LLONG_MIN <= INT_LEAST8_MIN && INT_LEAST8_MAX <= LLONG_MAX
# define __FMTdLEAST8 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_LEAST16_MIN && INT_LEAST16_MAX <= INT_MAX
# define __FMTdLEAST16 "d"
#elif LONG_MIN <= INT_LEAST16_MIN && INT_LEAST16_MAX <= LONG_MAX
# define __FMTdLEAST16 "ld"
#elif LLONG_MIN <= INT_LEAST16_MIN && INT_LEAST16_MAX <= LLONG_MAX
# define __FMTdLEAST16 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_LEAST32_MIN && INT_LEAST32_MAX <= INT_MAX
# define __FMTdLEAST32 "d"
#elif LONG_MIN <= INT_LEAST32_MIN && INT_LEAST32_MAX <= LONG_MAX
# define __FMTdLEAST32 "ld"
#elif LLONG_MIN <= INT_LEAST32_MIN && INT_LEAST32_MAX <= LLONG_MAX
# define __FMTdLEAST32 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_LEAST64_MIN && INT_LEAST64_MAX <= INT_MAX
# define __FMTdLEAST64 "d"
#elif LONG_MIN <= INT_LEAST64_MIN && INT_LEAST64_MAX <= LONG_MAX
# define __FMTdLEAST64 "ld"
#elif LLONG_MIN <= INT_LEAST64_MIN && INT_LEAST64_MAX <= LLONG_MAX
# define __FMTdLEAST64 "lld"
#else
# error "WTF?"
#endif

#if UINT_LEAST8_MAX <= UINT_MAX
# define __FMTuLEAST8 "u"
# define __FMToLEAST8 "o"
# define __FMTxLEAST8 "x"
# define __FMTXLEAST8 "X"
#elif UINT_LEAST8_MAX <= ULONG_MAX
# define __FMTuLEAST8 "lu"
# define __FMToLEAST8 "lo"
# define __FMTxLEAST8 "lx"
# define __FMTXLEAST8 "lX"
#elif UINT_LEAST8_MAX <= ULLONG_MAX
# define __FMTuLEAST8 "llu"
# define __FMToLEAST8 "llo"
# define __FMTxLEAST8 "llx"
# define __FMTXLEAST8 "llX"
#else
# error "WTF?"
#endif

#if UINT_LEAST16_MAX <= UINT_MAX
# define __FMTuLEAST16 "u"
# define __FMToLEAST16 "o"
# define __FMTxLEAST16 "x"
# define __FMTXLEAST16 "X"
#elif UINT_LEAST16_MAX <= ULONG_MAX
# define __FMTuLEAST16 "lu"
# define __FMToLEAST16 "lo"
# define __FMTxLEAST16 "lx"
# define __FMTXLEAST16 "lX"
#elif UINT_LEAST16_MAX <= ULLONG_MAX
# define __FMTuLEAST16 "llu"
# define __FMToLEAST16 "llo"
# define __FMTxLEAST16 "llx"
# define __FMTXLEAST16 "llX"
#else
# error "WTF?"
#endif

#if UINT_LEAST32_MAX <= UINT_MAX
# define __FMTuLEAST32 "u"
# define __FMToLEAST32 "o"
# define __FMTxLEAST32 "x"
# define __FMTXLEAST32 "X"
#elif UINT_LEAST32_MAX <= ULONG_MAX
# define __FMTuLEAST32 "lu"
# define __FMToLEAST32 "lo"
# define __FMTxLEAST32 "lx"
# define __FMTXLEAST32 "lX"
#elif UINT_LEAST32_MAX <= ULLONG_MAX
# define __FMTuLEAST32 "llu"
# define __FMToLEAST32 "llo"
# define __FMTxLEAST32 "llx"
# define __FMTXLEAST32 "llX"
#else
# error "WTF?"
#endif

#if UINT_LEAST64_MAX <= UINT_MAX
# define __FMTuLEAST64 "u"
# define __FMToLEAST64 "o"
# define __FMTxLEAST64 "x"
# define __FMTXLEAST64 "X"
#elif UINT_LEAST64_MAX <= ULONG_MAX
# define __FMTuLEAST64 "lu"
# define __FMToLEAST64 "lo"
# define __FMTxLEAST64 "lx"
# define __FMTXLEAST64 "lX"
#elif UINT_LEAST64_MAX <= ULLONG_MAX
# define __FMTuLEAST64 "llu"
# define __FMToLEAST64 "llo"
# define __FMTxLEAST64 "llx"
# define __FMTXLEAST64 "llX"
#else
# error "WTF?"
#endif

#define PRIdLEAST8  __FMTdLEAST8
#define PRIdLEAST16 __FMTdLEAST16
#define PRIdLEAST32 __FMTdLEAST32
#define PRIdLEAST64 __FMTdLEAST64

#define PRIiLEAST8  __FMTdLEAST8
#define PRIiLEAST16 __FMTdLEAST16
#define PRIiLEAST32 __FMTdLEAST32
#define PRIiLEAST64 __FMTdLEAST64

#define PRIuLEAST8  __FMTuLEAST8
#define PRIuLEAST16 __FMTuLEAST16
#define PRIuLEAST32 __FMTuLEAST32
#define PRIuLEAST64 __FMTuLEAST64

#define PRIoLEAST8  __FMToLEAST8
#define PRIoLEAST16 __FMToLEAST16
#define PRIoLEAST32 __FMToLEAST32
#define PRIoLEAST64 __FMToLEAST64

#define PRIxLEAST8  __FMTxLEAST8
#define PRIxLEAST16 __FMTxLEAST16
#define PRIxLEAST32 __FMTxLEAST32
#define PRIxLEAST64 __FMTxLEAST64

#define PRIXLEAST8  __FMTXLEAST8
#define PRIXLEAST16 __FMTXLEAST16
#define PRIXLEAST32 __FMTXLEAST32
#define PRIXLEAST64 __FMTXLEAST64

#define SCNdLEAST8  __FMTdLEAST8
#define SCNdLEAST16 __FMTdLEAST16
#define SCNdLEAST32 __FMTdLEAST32
#define SCNdLEAST64 __FMTdLEAST64

#define SCNiLEAST8  __FMTdLEAST8
#define SCNiLEAST16 __FMTdLEAST16
#define SCNiLEAST32 __FMTdLEAST32
#define SCNiLEAST64 __FMTdLEAST64

#define SCNuLEAST8  __FMTuLEAST8
#define SCNuLEAST16 __FMTuLEAST16
#define SCNuLEAST32 __FMTuLEAST32
#define SCNuLEAST64 __FMTuLEAST64

#define SCNoLEAST8  __FMToLEAST8
#define SCNoLEAST16 __FMToLEAST16
#define SCNoLEAST32 __FMToLEAST32
#define SCNoLEAST64 __FMToLEAST64

#define SCNxLEAST8  __FMTxLEAST8
#define SCNxLEAST16 __FMTxLEAST16
#define SCNxLEAST32 __FMTxLEAST32
#define SCNxLEAST64 __FMTxLEAST64

#if INT_MIN <= INT_FAST8_MIN && INT_FAST8_MAX <= INT_MAX
# define __FMTdFAST8 "d"
#elif LONG_MIN <= INT_FAST8_MIN && INT_FAST8_MAX <= LONG_MAX
# define __FMTdFAST8 "ld"
#elif LLONG_MIN <= INT_FAST8_MIN && INT_FAST8_MAX <= LLONG_MAX
# define __FMTdFAST8 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_FAST16_MIN && INT_FAST16_MAX <= INT_MAX
# define __FMTdFAST16 "d"
#elif LONG_MIN <= INT_FAST16_MIN && INT_FAST16_MAX <= LONG_MAX
# define __FMTdFAST16 "ld"
#elif LLONG_MIN <= INT_FAST16_MIN && INT_FAST16_MAX <= LLONG_MAX
# define __FMTdFAST16 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_FAST32_MIN && INT_FAST32_MAX <= INT_MAX
# define __FMTdFAST32 "d"
#elif LONG_MIN <= INT_FAST32_MIN && INT_FAST32_MAX <= LONG_MAX
# define __FMTdFAST32 "ld"
#elif LLONG_MIN <= INT_FAST32_MIN && INT_FAST32_MAX <= LLONG_MAX
# define __FMTdFAST32 "lld"
#else
# error "WTF?"
#endif

#if INT_MIN <= INT_FAST64_MIN && INT_FAST64_MAX <= INT_MAX
# define __FMTdFAST64 "d"
#elif LONG_MIN <= INT_FAST64_MIN && INT_FAST64_MAX <= LONG_MAX
# define __FMTdFAST64 "ld"
#elif LLONG_MIN <= INT_FAST64_MIN && INT_FAST64_MAX <= LLONG_MAX
# define __FMTdFAST64 "lld"
#else
# error "WTF?"
#endif

#if UINT_FAST8_MAX <= UINT_MAX
# define __FMTuFAST8 "u"
# define __FMToFAST8 "o"
# define __FMTxFAST8 "x"
# define __FMTXFAST8 "X"
#elif UINT_FAST8_MAX <= ULONG_MAX
# define __FMTuFAST8 "lu"
# define __FMToFAST8 "lo"
# define __FMTxFAST8 "lx"
# define __FMTXFAST8 "lX"
#elif UINT_FAST8_MAX <= ULLONG_MAX
# define __FMTuFAST8 "llu"
# define __FMToFAST8 "llo"
# define __FMTxFAST8 "llx"
# define __FMTXFAST8 "llX"
#else
# error "WTF?"
#endif

#if UINT_FAST16_MAX <= UINT_MAX
# define __FMTuFAST16 "u"
# define __FMToFAST16 "o"
# define __FMTxFAST16 "x"
# define __FMTXFAST16 "X"
#elif UINT_FAST16_MAX <= ULONG_MAX
# define __FMTuFAST16 "lu"
# define __FMToFAST16 "lo"
# define __FMTxFAST16 "lx"
# define __FMTXFAST16 "lX"
#elif UINT_FAST16_MAX <= ULLONG_MAX
# define __FMTuFAST16 "llu"
# define __FMToFAST16 "llo"
# define __FMTxFAST16 "llx"
# define __FMTXFAST16 "llX"
#else
# error "WTF?"
#endif

#if UINT_FAST32_MAX <= UINT_MAX
# define __FMTuFAST32 "u"
# define __FMToFAST32 "o"
# define __FMTxFAST32 "x"
# define __FMTXFAST32 "X"
#elif UINT_FAST32_MAX <= ULONG_MAX
# define __FMTuFAST32 "lu"
# define __FMToFAST32 "lo"
# define __FMTxFAST32 "lx"
# define __FMTXFAST32 "lX"
#elif UINT_FAST32_MAX <= ULLONG_MAX
# define __FMTuFAST32 "llu"
# define __FMToFAST32 "llo"
# define __FMTxFAST32 "llx"
# define __FMTXFAST32 "llX"
#else
# error "WTF?"
#endif

#if UINT_FAST64_MAX <= UINT_MAX
# define __FMTuFAST64 "u"
# define __FMToFAST64 "o"
# define __FMTxFAST64 "x"
# define __FMTXFAST64 "X"
#elif UINT_FAST64_MAX <= ULONG_MAX
# define __FMTuFAST64 "lu"
# define __FMToFAST64 "lo"
# define __FMTxFAST64 "lx"
# define __FMTXFAST64 "lX"
#elif UINT_FAST64_MAX <= ULLONG_MAX
# define __FMTuFAST64 "llu"
# define __FMToFAST64 "llo"
# define __FMTxFAST64 "llx"
# define __FMTXFAST64 "llX"
#else
# error "WTF?"
#endif

#define PRIdFAST8  __FMTdFAST8
#define PRIdFAST16 __FMTdFAST16
#define PRIdFAST32 __FMTdFAST32
#define PRIdFAST64 __FMTdFAST64

#define PRIiFAST8  __FMTdFAST8
#define PRIiFAST16 __FMTdFAST16
#define PRIiFAST32 __FMTdFAST32
#define PRIiFAST64 __FMTdFAST64

#define PRIuFAST8  __FMTuFAST8
#define PRIuFAST16 __FMTuFAST16
#define PRIuFAST32 __FMTuFAST32
#define PRIuFAST64 __FMTuFAST64

#define PRIoFAST8  __FMToFAST8
#define PRIoFAST16 __FMToFAST16
#define PRIoFAST32 __FMToFAST32
#define PRIoFAST64 __FMToFAST64

#define PRIxFAST8  __FMTxFAST8
#define PRIxFAST16 __FMTxFAST16
#define PRIxFAST32 __FMTxFAST32
#define PRIxFAST64 __FMTxFAST64

#define PRIXFAST8  __FMTXFAST8
#define PRIXFAST16 __FMTXFAST16
#define PRIXFAST32 __FMTXFAST32
#define PRIXFAST64 __FMTXFAST64

#define SCNdFAST8  __FMTdFAST8
#define SCNdFAST16 __FMTdFAST16
#define SCNdFAST32 __FMTdFAST32
#define SCNdFAST64 __FMTdFAST64

#define SCNiFAST8  __FMTdFAST8
#define SCNiFAST16 __FMTdFAST16
#define SCNiFAST32 __FMTdFAST32
#define SCNiFAST64 __FMTdFAST64

#define SCNuFAST8  __FMTuFAST8
#define SCNuFAST16 __FMTuFAST16
#define SCNuFAST32 __FMTuFAST32
#define SCNuFAST64 __FMTuFAST64

#define SCNoFAST8  __FMToFAST8
#define SCNoFAST16 __FMToFAST16
#define SCNoFAST32 __FMToFAST32
#define SCNoFAST64 __FMToFAST64

#define SCNxFAST8  __FMTxFAST8
#define SCNxFAST16 __FMTxFAST16
#define SCNxFAST32 __FMTxFAST32
#define SCNxFAST64 __FMTxFAST64

#if WITH_FIXED_WIDTH_TYPES

# if INT_MIN <= INT8_MIN && INT8_MAX <= INT_MAX
#  define __FMTd8 "d"
# elif LONG_MIN <= INT8_MIN && INT8_MAX <= LONG_MAX
#  define __FMTd8 "ld"
# elif LLONG_MIN <= INT8_MIN && INT8_MAX <= LLONG_MAX
#  define __FMTd8 "lld"
# else
#  error "WTF?"
# endif

# if INT_MIN <= INT16_MIN && INT16_MAX <= INT_MAX
#  define __FMTd16 "d"
# elif LONG_MIN <= INT16_MIN && INT16_MAX <= LONG_MAX
#  define __FMTd16 "ld"
# elif LLONG_MIN <= INT16_MIN && INT16_MAX <= LLONG_MAX
#  define __FMTd16 "lld"
# else
#  error "WTF?"
# endif

# if INT_MIN <= INT32_MIN && INT32_MAX <= INT_MAX
#  define __FMTd32 "d"
# elif LONG_MIN <= INT32_MIN && INT32_MAX <= LONG_MAX
#  define __FMTd32 "ld"
# elif LLONG_MIN <= INT32_MIN && INT32_MAX <= LLONG_MAX
#  define __FMTd32 "lld"
# else
#  error "WTF?"
# endif

# if INT_MIN <= INT64_MIN && INT64_MAX <= INT_MAX
#  define __FMTd64 "d"
# elif LONG_MIN <= INT64_MIN && INT64_MAX <= LONG_MAX
#  define __FMTd64 "ld"
# elif LLONG_MIN <= INT64_MIN && INT64_MAX <= LLONG_MAX
#  define __FMTd64 "lld"
# else
#  error "WTF?"
# endif

# if UINT8_MAX <= UINT_MAX
#  define __FMTu8 "u"
#  define __FMTx8 "x"
#  define __FMTX8 "X"
# elif UINT8_MAX <= ULONG_MAX
#  define __FMTu8 "lu"
#  define __FMTx8 "lx"
#  define __FMTX8 "lX"
# elif UINT8_MAX <= ULLONG_MAX
#  define __FMTu8 "llu"
#  define __FMTx8 "llx"
#  define __FMTX8 "llX"
# else
#  error "WTF?"
# endif

# if UINT16_MAX <= UINT_MAX
#  define __FMTu16 "u"
#  define __FMTx16 "x"
#  define __FMTX16 "X"
# elif UINT32_MAX <= ULONG_MAX
#  define __FMTu16 "lu"
#  define __FMTx16 "lx"
#  define __FMTX16 "lX"
# elif UINT64_MAX <= ULLONG_MAX
#  define __FMTu16 "llu"
#  define __FMTx16 "llx"
#  define __FMTX16 "llX"
# else
#  error "WTF?"
# endif

# if UINT32_MAX <= UINT_MAX
#  define __FMTu32 "u"
#  define __FMTx32 "x"
#  define __FMTX32 "X"
# elif UINT32_MAX <= ULONG_MAX
#  define __FMTu32 "lu"
#  define __FMTx32 "lx"
#  define __FMTX32 "lX"
# elif UINT32_MAX <= ULLONG_MAX
#  define __FMTu32 "llu"
#  define __FMTx32 "llx"
#  define __FMTX32 "llX"
# else
#  error "WTF?"
# endif

# if UINT64_MAX <= UINT_MAX
#  define __FMTu64 "u"
#  define __FMTx64 "x"
#  define __FMTX64 "X"
# elif UINT64_MAX <= ULONG_MAX
#  define __FMTu64 "lu"
#  define __FMTx64 "lx"
#  define __FMTX64 "lX"
# elif UINT64_MAX <= ULLONG_MAX
#  define __FMTu64 "llu"
#  define __FMTx64 "llx"
#  define __FMTX64 "llX"
# else
#  error "WTF?"
# endif

# define PRId8  __FMTd8
# define PRId16 __FMTd16
# define PRId32 __FMTd32

# define PRIi8  __FMTd8
# define PRIi16 __FMTd16
# define PRIi32 __FMTd32

# define PRIu8  __FMTu8
# define PRIu16 __FMTu16
# define PRIu32 __FMTu32

# define PRIo8  __FMTo8
# define PRIo16 __FMTo16
# define PRIo32 __FMTo32

# define PRIx8  __FMTx8
# define PRIx16 __FMTx16
# define PRIx32 __FMTx32

# define PRIX8  __FMTX8
# define PRIX16 __FMTX16
# define PRIX32 __FMTX32

# if WITH_64BIT_FIXED_WIDTH_TYPES
#  define PRId64 __FMTd64
#  define PRIi64 __FMTd64
#  define PRIu64 __FMTu64
#  define PRIo64 __FMTo64
#  define PRIx64 __FMTx64
#  define PRIX64 __FMTX64
# endif

# define SCNd8  __FMTd8
# define SCNd16 __FMTd16
# define SCNd32 __FMTd32

# define SCNi8  __FMTd8
# define SCNi16 __FMTd16
# define SCNi32 __FMTd32

# define SCNu8  __FMTu8
# define SCNu16 __FMTu16
# define SCNu32 __FMTu32

# define SCNo8  __FMTo8
# define SCNo16 __FMTo16
# define SCNo32 __FMTo32

# define SCNx8  __FMTx8
# define SCNx16 __FMTx16
# define SCNx32 __FMTx32

# if WITH_64BIT_FIXED_WIDTH_TYPES
#  define SCNd64 __FMTd64
#  define SCNi64 __FMTd64
#  define SCNu64 __FMTu64
#  define SCNo64 __FMTo64
#  define SCNx64 __FMTx64
# endif

#endif // WITH_FIXED_WIDTH_TYPES

#if WITH_INTPTR_TYPES

# if INT_MIN <= INTPTR_MIN && INTPTR_MAX <= INT_MAX
#  define __FMTdPTR "d"
# elif LONG_MIN <= INTPTR_MIN && INTPTR_MAX <= LONG_MAX
#  define __FMTdPTR "ld"
# elif LLONG_MIN <= INTPTR_MIN && INTPTR_MAX <= LLONG_MAX
#  define __FMTdPTR "lld"
# else
#  error "WTF?"
# endif

# if UINTPTR_MAX <= UINT_MAX
#  define __FMTuPTR "u"
#  define __FMToPTR "o"
#  define __FMTxPTR "x"
#  define __FMTXPTR "X"
# elif UINTPTR_MAX <= ULONG_MAX
#  define __FMTuPTR "lu"
#  define __FMToPTR "lo"
#  define __FMTxPTR "lx"
#  define __FMTXPTR "lX"
# elif UINTPTR_MAX <= ULLONG_MAX
#  define __FMTuPTR "llu"
#  define __FMToPTR "llo"
#  define __FMTxPTR "llx"
#  define __FMTXPTR "llX"
# else
#  error "WTF?"
# endif

# define PRIdPTR __FMTdPTR
# define PRIiPTR __FMTdPTR
# define PRIuPTR __FMTuPTR

# define SCNdPTR __FMTdPTR
# define SCNiPTR __FMTdPTR
# define SCNuPTR __FMTuPTR

#endif // WITH_INTPTR_TYPES

#if INT_MIN <= INTMAX_MIN && INTMAX_MAX <= INT_MAX
# define __FMTdMAX "d"
#elif LONG_MIN <= INTMAX_MIN && INTMAX_MAX <= LONG_MAX
# define __FMTdMAX "ld"
#elif LLONG_MIN <= INTMAX_MIN && INTMAX_MAX <= LLONG_MAX
# define __FMTdMAX "lld"
#else
# error "WTF?"
#endif

#if UINTMAX_MAX <= UINT_MAX
# define __FMTuMAX "u"
# define __FMToMAX "o"
# define __FMTxMAX "x"
# define __FMTXMAX "X"
#elif UINTMAX_MAX <= ULONG_MAX
# define __FMTuMAX "lu"
# define __FMToMAX "lo"
# define __FMTxMAX "lx"
# define __FMTXMAX "lX"
#elif UINTMAX_MAX <= ULLONG_MAX
# define __FMTuMAX "llu"
# define __FMToMAX "llo"
# define __FMTxMAX "llx"
# define __FMTXMAX "llX"
#else
# error "WTF?"
#endif

#define PRIdMAX __FMTdMAX
#define PRIiMAX __FMTiMAX
#define PRIuMAX __FMTuMAX
#define PRIoMAX __FMToMAX
#define PRIxMAX __FMTxMAX
#define PRIXMAX __FMTXMAX

#define SCNdMAX __FMTdMAX
#define SCNiMAX __FMTiMAX
#define SCNuMAX __FMTuMAX
#define SCNoMAX __FMToMAX
#define SCNxMAX __FMTxMAX

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_INTTYPES_H */
