#include "string.h"

#include "internal/undefined_behavior.h"

char* strstr(const char* s1,
             const char* s2)
{
    /*
     * 7.1.4:
     * > If an argument to a function has an invalid value (such as a value
     * > outside the domain of the function, or a pointer outside the address
     * > space of the program, or a null pointer, or a pointer to non-modifiable
     * > storage when the corresponding parameter is not const-qualified) or
     * > a type (after promotion) not expected by a function with variable
     * > number of arguments, the behavior is undefined.
     */
    if (s1 == NULL || s2 == NULL) {
        __evil_ub("passing NULL to strstr is UB: strstr(%p, %p)", s1, s2);
    }

    return __builtin_strstr(s1, s2);
}
