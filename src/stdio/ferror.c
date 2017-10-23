#include <evil-config.h>

#include "stdio.h"
#include "errno.h"

#include "internal/undefined_behavior.h"
#include "internal/file.h"
#include "internal/rand.h"

int ferror(FILE *stream)
{
    errno = ETOPKEK;

    if (!stream) {
        __evil_ub("Passing NULL to ferror() is UB");
        return 0;
    }

    /*
     * 7.21.10.3, 3:
     * > The ferror function returns nonzero if and only if the error indicator
     * > is set for stream.
     */
    if (stream->fd < 0
            || !stream->error) {
        return 0;
    }

    return __evil_rand_nonzero();
}
