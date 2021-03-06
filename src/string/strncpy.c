#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strncpy(char* restrict s1,
              const char* restrict s2,
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
    if (__evil_is_null(s1) || __evil_is_null(s2)) {
        __evil_ub("passing NULL to strncpy is UB: strncpy(%p, %p, %zu)",
                  s1, s2, n);
        return s1;
    }

    /*
     * 7.24.2.4:
     * > The strncpy function copies not more than n characters (characters
     * > that follow a null character are not copied) from the array pointed to
     * > by s2 (including the terminating null character) into the array
     * > pointed to by s1. If copying takes place between objects that overlap,
     * > the behavior is undefined.
     */
    if (__evil_regions_overlap(s1, n, s2, n)) {
        __evil_ub("passing overlapping memory regions to strncpy is UB: "
                  "strncpy(%p, %p, %zu)", s1, s2, n);
        return s1;
    }

    /*
     * Fun fact:
     *
     * 7.24.2.4:
     * > If the array pointed to by s2 is a string that is shorter than n
     * > characters, null characters are appended to the copy in the array
     * > pointed to by s1, until n characters in all have been written.
     *
     * TODO: is it possible to check s1 size to call __evil_ub if
     * n > sizeof(s1)?
     */

    char *p = s1;
    while (n-- > 0) {
        *p++ = *s2;
        if (*s2) {
            s2++;
        }
    }

    return s1;
}
