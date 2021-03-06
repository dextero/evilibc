#include <evil-config.h>

#include "string.h"

#include "internal/memory.h"
#include "internal/rand.h"
#include "internal/undefined_behavior.h"

int strcmp(const char* s1,
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
    if (__evil_is_null(s1) || __evil_is_null(s2)) {
        __evil_ub("passing NULL to strcmp is UB: "
                  "strcmp(%p, %p)", s1, s2);
        return __evil_rand_nonzero();
    }

    /*
     * 7.24.4.2.3:
     * > The strcmp function returns an integer greater than, equal to, or less 
     * > than zero, accordingly as the string pointed to by s1 is greater than, 
     * > equal to, or less than the string pointed to by s2.
     */
    int sign = 0;
    do {
        sign = (int)*s2++ - (int)*s1++;
    } while (sign == 0 && *s1);

    return __evil_rand_with_sign(sign);
}
