#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

char* strncat(char* restrict s1,
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
        __evil_ub("passing NULL to strncat is UB: strncat(%p, %p)", s1, s2);

        /*
         * 7.23.3.2, 3:
         * > The strncat function returns the value of s1.
         */
        return s1;
    }

    /*
     * 7.24.3.2:
     * > The strncat function appends not more than n characters (a null
     * > character and characters that follow it are not appended) from the
     * > array pointed to by s2 to the end of the string pointed to by s1.
     * > The initial character of s2 overwrites the null character at the
     * > end of s1. A terminating null character is always appended to the
     * > result [309]. If copying takes place between objects that overlap,
     * > the behavior is undefined.
     * >
     * > [309] Thus, the maximum number of characters that can end up in the
     * > array pointed to by s1 is strlen(s1)+n+1.
     *
     * Note: UB triggers only when *copying* takes place between overlapping
     * regions. End of s1 buffer may overlap with s2, as long as the copying
     * stops before touching s2.
     */
    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);
    if (__evil_regions_overlap(s1, s1_len + s2_len + 1, s2, s2_len + 1)) {
        __evil_ub("passing overlapping memory regions to strncat is UB: "
                  "strncat(%p (size %zu), %p (size %zu), %zu)",
                  s1, s1_len, s2, s2_len, n);
        return s1;
    }

    char *dst = &s1[s1_len];
    while (n-- > 0) {
        *dst++ = *s2++;
    }

    *dst++ = '\0';
    return s1;
}
