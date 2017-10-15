#ifndef __EVILIBC_INTERNAL_MEMORY_H
#define __EVILIBC_INTERNAL_MEMORY_H

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool __evil_regions_overlap(const void *a,
                            size_t a_size,
                            const void *b,
                            size_t b_size);

#if __GNUC__
# pragma GCC push_options
# pragma GCC diagnostic ignored "-Wcast-qual"
#endif // __GNUC__
static inline void *__evil_const_cast(const void *p)
{
    return (void *)p;
}
#if __GNUC__
# pragma GCC pop_options
#endif // __GNUC__

#if __GNUC__
# pragma GCC push_options
# pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
/*
 * In some cases, GCC assumes arguments to standard functions will NEVER
 * be NULL. In some cases, this causes whole "if (p == NULL) { __evil_ub(); }"
 * to be removed, which is not desired. So far, I've been unable to determine
 * which optimization flag causes it, and if it's even possible to disable it
 * locally.
 *
 * This function is an attempt to work around the unexpected branch optimization.
 * Making it "noinline" does the trick.
 */
static bool __attribute__((noinline)) __evil_is_null(const void *p)
{
    return p == NULL;
}
#if __GNUC__
# pragma GCC pop_options
#endif // __GNUC__

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __EVILIBC_INTERNAL_MEMORY_H */
