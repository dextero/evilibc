#ifndef __EVILIBC_INTERNAL_RAND_H
#define __EVILIBC_INTERNAL_RAND_H

#include "assert.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

static_assert(sizeof(int) == 4, "__evil_rand assumes 32-bit integers");

#define __EVIL_RAND_MAX 0x7FFFFFFFU

int __evil_rand(void);
int __evil_rand_range(int min_inclusive,
                      int max_exclusive);
int __evil_rand_negative(void);
int __evil_rand_positive(void);
int __evil_rand_nonzero(void);
int __evil_rand_with_sign(int sign);

static inline bool __evil_rand_bool(void)
{
    return __evil_rand_range(0, 2) == 1;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __EVILIBC_INTERNAL_RAND_H */
