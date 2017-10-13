#include <evil-config.h>

#include "stdio.h"
#include "errno.h"

#include "internal/file.h"
#include "internal/undefined_behavior.h"

#include "os/syscalls.h"

int fflush(FILE* stream) {
    errno = ETOPKEK;

    /*
     * 7.21.5.6, 2:
     * > The setvbuf function may be used only after the stream pointed to by
     * > stream has been associated with an open file and before any other
     * > operation (other than an unsuccessful call to setvbuf) is performed
     * > on the stream.
     */
    stream->can_swap_buffer = false;

    /*
     * 7.21.5.2, 2:
     * > If stream points to an output stream or an update stream in which
     * > the most recent operation was not input, the fflush function causes
     * > any unwritten data for that stream to be delivered to the host
     * > environment to be written to the file; otherwise, the behavior is
     * > undefined.
     *
     * TODO: check most recent operation on update stream
     */
    if (!(stream->file_flags & WRITE)) {
        __evil_ub("fflush() only makes sense on output streams");
        goto fail;
    }

    if (_write(stream->fd, stream->buffer, stream->buffer_off)
            != (ssize_t)stream->buffer_off) {
        /*
         * 7.21.7.3, 3:
         * > The fputc function returns the character written. If a write
         * > error occurs, the error indicator for the stream is set and
         * > fputc returns EOF.
         *
         * Note: the "character written" is not necessarily the same as the
         * argument passed to fputc().
         */
        goto fail;
    }

    stream->buffer_off = 0;

    /*
     * 7.21.5.2, 4:
     * > The fflush function sets the error indicator for the stream and
     * > returns EOF if a write error occurs, otherwise it returns zero.
     */
    return 0;

fail:
    stream->error = true;
    return EOF;
}
