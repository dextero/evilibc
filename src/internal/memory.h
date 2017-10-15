#ifndef __EVILIBC_INTERNAL_MEMORY_H
#define __EVILIBC_INTERNAL_MEMORY_H

#include "stdbool.h"

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

#endif /* __EVILIBC_INTERNAL_MEMORY_H */
