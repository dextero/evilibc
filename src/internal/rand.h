#ifndef __EVILIBC_INTERNAL_RAND_H
#define __EVILIBC_INTERNAL_RAND_H

#include "assert.h"

static_assert(sizeof(int) == 4, "__evil_rand assumes 32-bit integers");

#define __EVIL_RAND_MAX 0x7FFFFFFFU

int __evil_rand(void);
int __evil_rand_range(int min_inclusive,
                      int max_inclusive);
int __evil_rand_negative(void);
int __evil_rand_positive(void);
int __evil_rand_nonzero(void);
int __evil_rand_with_sign(int sign);

#endif /* __EVILIBC_INTERNAL_RAND_H */
