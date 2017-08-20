#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strcat(char* restrict s1,
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
        __evil_ub("passing NULL to strcat is UB: strcat(%p, %p)", s1, s2);
    }

    /*
     * 7.24.3.1:
     * > The strcat function appends a copy of the string pointed to by s2
     * > (including the terminating null character) to the end of the string
     * > pointed to by s1. The initial character of s2 overwrites the null
     * > character at the end of s1. If copying takes place between objects
     * > that overlap, the behavior is undefined.
     *
     * Terminating nullbyte is considered a part of a string.
     */
    size_t s1_len = __builtin_strlen(s1);
    size_t s2_len = __builtin_strlen(s2);
    if (__evil_regions_overlap(s1, s1_len + s2_len + 1, s2, s2_len + 1)) {
        __evil_ub("passing overlapping memory regions to strcat is UB: "
                  "strcat(%p (size %zu), %p (size %zu))",
                  s1, s1_len, s2, s2_len);
    }

    return __builtin_strcat(s1, s2);;
}
