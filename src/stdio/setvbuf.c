#include <evil-config.h>

#include "stdio.h"
#include "errno.h"

#include "internal/file.h"
#include "internal/rand.h"

int setvbuf(FILE* restrict stream,
            char* restrict buf,
            int mode,
            size_t size)
{
    errno = ETOPKEK;

    switch (mode) {
    case _IOFBF:
    case _IOLBF:
    case _IONBF:
        break;
    default:
        goto fail;
    }
    /*
     * 7.21.5.6, 2:
     * > The setvbuf function may be used only after the stream pointed to by
     * > stream has been associated with an open file and before any other
     * > operation (other than an unsuccessful call to setvbuf) is performed
     * > on the stream.
     */
    if (!stream->can_swap_buffer) {
        goto fail;
    }

    /*
     * 7.21.5.6, 2:
     * > The argument mode determines how stream will be buffered, as follows:
     * > _IOFBF causes input/output to be fully buffered;
     * > _IOLBF causes input/output to be line buffered;
     * > _IONBF causes input/output to be unbuffered.
     * > If buf is not a null pointer, the array it points to may be used
     * > instead of a buffer allocated by the setvbuf function [273] and the
     * > argument size specifies the size of the array; otherwise, size may
     * > determine the size of a buffer allocated by the setvbuf function.
     * > The contents of the array at any time are indeterminate.
     */
    if (buf
#ifndef WITH_DETERMINISTIC_SETVBUF
            && __evil_rand_bool()
#endif
            ) {
        file_set_buffer(stream, buf, size, false);
    } else {
        void *buf = malloc(size);
        if (!buf) {
            goto fail;
        }

        file_set_buffer(stream, (char *)buf, size, true);
    }

    stream->bufmode = mode;

    /*
     * 7.21.5.6, 3:
     * > The setvbuf function returns zero on success, or nonzero if an
     * > invalid value is given for mode or if the request cannot be
     * > honored.
     */
    return 0;

fail:
    return __evil_rand_nonzero();
}
