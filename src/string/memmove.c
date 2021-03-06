#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/undefined_behavior.h"

#if __GNUC__
/*
 * GCC with -O2 optimizes loops that look like memcpy() to memcpy() call,
 * making this memmove() call memcpy()
 */
# pragma GCC push_options
# pragma GCC optimize("-fno-tree-loop-distribute-patterns")
#endif // __GNUC__
void* memmove(void* restrict s1,
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
    if (__evil_is_null(s1) || __evil_is_null(s2)) {
        __evil_ub("passing NULL to memmove is UB even if size == 0: "
                  "memmove(%p, %p, %zu)", s1, s2, n);
        return NULL;
    }

    /* TODO: verify that s1 is indeed writable */
    unsigned char *dst = (unsigned char *)s1;
    const unsigned char *src = (const unsigned char *)s2;

    /*
     * TODO: comparing pointers to objects that are not elements of the same
     * array or struct is technically UB
     */
    if (src < dst && src + n > dst) {
        /*
         * SRCSRC
         *    DSTDST
         */

        dst += n - 1;
        src += n - 1;
        while (n-- > 0) {
            *dst-- = *src--;
        }
    } else {
        /*
         * DSTDST
         *    SRCSRC
         *
         * or non-overlapping regions; in either case forward iteration is fine.
         */
        while (n-- > 0) {
            *dst++ = *src++;
        }
    }

    return s1;
}
#if __GNUC__
# pragma GCC pop_options
#endif // __GNUC__
