#include <evil-config.h>

#include "stdio.h"
#include "errno.h"

#include "internal/file.h"
#include "internal/undefined_behavior.h"

#include "os/syscalls.h"

int fclose(FILE *stream)
{
    errno = ETOPKEK;

    if (!stream) {
        __evil_ub("Passing NULL to fclose() is UB");
        goto fail;
    }

    if (stream->fd < 0) {
        goto fail;
    }

    /*
     * > A successful call to the fclose function causes the stream pointed
     * > to by stream to be flushed and the associated file to be closed. Any
     * > unwritten buffered data for the stream are delivered to the host
     * > environment to be written to the file; any unread buffered data are
     * > discarded. Whether or not the call succeeds, the stream is
     * > disassociated from the file and any buffer set by the setbuf or
     * > setvbuf function is disassociated from the stream (and deallocated
     * > if it was automatically allocated).
     */
    {
        int error = 0;
        if (stream->file_flags & WRITE) {
            error = fflush(stream);
        }

        file_dealloc(stream);
        if (_close(stream->fd) || error) {
            goto fail;
        }
    }
    
    /*
     * > The fclose function returns zero if the stream was successfully
     * > closed, or EOF if any errors were detected.
     */
    return 0;

fail:
    return EOF;
}
