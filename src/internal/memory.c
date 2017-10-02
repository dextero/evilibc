#include <evil-config.h>

#include "internal/memory.h"

bool __evil_regions_overlap(const void *a,
                            size_t a_size,
                            const void *b,
                            size_t b_size)
{
    const void* a_end = (const char*)a + a_size;
    const void* b_end = (const char*)b + b_size;

    return (a < b && b < a_end)
        || (a < b_end && b_end < a_end)
        || (b < a && a < b_end)
        || (b < a_end && a_end < b_end);
}

