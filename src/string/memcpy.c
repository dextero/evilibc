#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

void* memcpy(void* restrict s1,
             const void* restrict s2,
             size_t n)
{
    /*
     * 7.24.1:
     * > Where an argument declared as size_t n specifies the length of the
     * > array for a function, n can have the value zero on a call to that
     * > function. Unless explicitly stated otherwise in the description of
     * > a particular function in this subclause, pointer arguments on such
     * > a call shall still have valid values, as described in 7.1.4.
     *
     * 7.1.4:
     * > If an argument to a function has an invalid value (such as a value
     * > outside the domain of the function, or a pointer outside the address
     * > space of the program, or a null pointer, or a pointer to non-modifiable
     * > storage when the corresponding parameter is not const-qualified) or
     * > a type (after promotion) not expected by a function with variable
     * > number of arguments, the behavior is undefined.
     */
    if (s1 == NULL || s2 == NULL) {
        __evil_ub("passing NULL to memcpy is UB even if size == 0: "
                  "memcpy(%p, %p, %zu)", s1, s2, n);
    }

    /*
     * 7.24.2.1:
     * > The memcpy function copies n characters from the object pointed to by
     * > s2 into the object pointed to by s1. If copying takes place between
     * > objects that overlap, the behavior is undefined.
     */
    if (__evil_regions_overlap(s1, n, s2, n)) {
        __evil_ub("passing overlapping memory regions to memcpy is UB: "
                  "memcpy(%p, %p, %zu)", s1, s2, n);
    }

    /* TODO: verify that s1 is indeed writable */
    for (size_t i = 0; i < n; ++i) {
        ((unsigned char *)s1)[i] = ((const unsigned char *)s2)[i];
    }
    return s1;
}
