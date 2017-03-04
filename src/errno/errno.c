#include "errno.h"

#include "internal/undefined_behavior.h"

_Thread_local int _errno = 0;

int *__evil_errno(void)
{
    /* sanity check; spec defines errno as a positive value */
    if (_errno < 0) {
        __evil_ub("errno has a negative value (%d)", _errno);
    }

    return &_errno;
}
