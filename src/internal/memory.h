#ifndef __EVILIBC_INTERNAL_MEMORY_H
#define __EVILIBC_INTERNAL_MEMORY_H

#include "stdbool.h"

bool __evil_regions_overlap(const void *a,
                            size_t a_size,
                            const void *b,
                            size_t b_size);

#endif /* __EVILIBC_INTERNAL_MEMORY_H */
