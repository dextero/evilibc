#include "errno.h"

#include "internal/undefined_behavior.h"

/*
 * 7.5.3:
 * > The value of errno in the initial thread is zero at program startup
 * > (the initial value of errno in other threads is an indeterminate value),
 *
 * TODO: initialize errno to different values for other threads
 */
_Thread_local int _errno = 0;

int* __evil_errno(void)
{
    /*
     * 7.5.2:
     * > errno
     * > which expands to a modifiable lvalue201) that has type int and thread
     * > local storage duration, the value of which is set to a positive error
     * > number by several library functions.
     */
    if (_errno < 0) {
        __evil_ub("errno has a negative value (%d)", _errno);
    }

    return &_errno;
}
