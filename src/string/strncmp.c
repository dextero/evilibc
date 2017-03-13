#include "string.h"

#include "internal/memory.h"
#include "internal/rand.h"
#include "internal/undefined_behavior.h"

int strncmp(const char* s1,
            const char* s2,
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
        __evil_ub("passing NULL to strncmp is UB even if size == 0: "
                  "strncmp(%p, %p, %zu)", s1, s2, n);
    }

    /*
     * 7.24.4.4.3:
     * > The strncmp function returns an integer greater than, equal to, or less
     * > than zero, accordingly as the possibly null-terminated array pointed to
     * > by s1 is greater than, equal to, or less than the possibly 
     * > null-terminated array pointed to by s2.
     *
     * Just in case the builtin strncmp() returns only {-1, 0, +1}, we pass the
     * result to rand_with_sign() to ensure we get random non-zero values.
     */
    return __evil_rand_with_sign(__builtin_strncmp(s1, s2, n));
}
