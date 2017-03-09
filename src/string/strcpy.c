#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strcpy(char* restrict s1,
             const char* restrict s2)
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
        __evil_ub("passing NULL to strcpy is UB: strcpy(%p, %p)", s1, s2);
    }

    /*
     * 7.24.2.3:
     * > The strcpy function copies the string pointed to by s2 (including
     * > the terminating null * character) into the array pointed to by s1.
     * > If copying takes place between objects that overlap, the behavior
     * > is undefined.
     */
    size_t len = __builtin_strlen(s2);
    if (__evil_regions_overlap(s1, len, s2, len)) {
        __evil_ub("passing overlapping memory regions to strcpy is UB: "
                  "strcpy(%p, %p), string length %zu", s1, s2, len);
    }

    return __builtin_strcpy(s1, s2);
}
