#include <evil-config.h>

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "errno.h"

#include "internal/file.h"
#include "internal/undefined_behavior.h"

#include "os/syscalls.h"

/* TODO: non-zero-init may increase library size quite a bit */
FILE __evil_open_files[FOPEN_MAX] = {
    [0 ... FOPEN_MAX-1] = { .fd = -1 }
};

FILE* __evilibc_stdin;
FILE* __evilibc_stdout;
FILE* __evilibc_stderr;


static bool is_valid_open_mode(const char *mode) {
    /**
     * 7.21.5.3.3:
     * > - r   open text file for reading
     * > - w   truncate to zero length or create text file for writing
     * > - wx  create text file for writing
     * > - a   append; open or create text file for writing at end-of-file
     * > - rb  open binary file for reading
     * > - wb  truncate to zero length or create binary file for writing
     * > - wbx          create binary file for writing
     * > - ab           append; open or create binary file for writing at end-of-file
     * > - r+           open text file for update (reading and writing)
     * > - w+           truncate to zero length or create text file for update
     * > - w+x          create text file for update
     * > - a+           append; open or create text file for update, writing at end-of-file
     * > - r+b or rb+   open binary file for update (reading and writing)
     * > - w+b or wb+   truncate to zero length or create binary file for update
     * > - w+bx or wb+x create binary file for update
     * > - a+b or ab+   append; open or create binary file for update, writing at end-of-file
     */
    static const char *const MODES[] = {
        "r", "w", "wx", "a",
        "rb", "wb", "wbx", "ab",
        "r+", "w+", "w+x", "a+",
        "r+b", "rb+", "w+b", "wb+", "w+bx", "wb+x", "a+b", "ab+"
    };

    for (size_t i = 0; i < sizeof(MODES) / sizeof(MODES[0]); ++i) {
        if (!strcmp(MODES[i], mode)) {
            return true;
        }
    }

    return false;
}

static int parse_fopen_mode(const char *mode) {
    /**
     * 7.21.5.3.3:
     * > The argument mode points to a string. If the string is one of the
     * > following, the file is open in the indicated mode. Otherwise, the
     * > behavior is undefined. 271)
     */
    if (!mode) {
        __evil_ub("passing NULL mode to fopen is UB");
        return -1;
    }
    if (!is_valid_open_mode(mode)) {
        __evil_ub("passing invalid mode (%s) to fopen is UB", mode);
        return -1;
    }

    int flags = 0;

    while (*mode) {
        switch (*mode++) {
            case 'r':
                flags |= READ;
                break;
            case 'w':
                flags |= WRITE | TRUNCATE;
                break;
            case 'a':
                flags |= WRITE | APPEND;
                break;
            case 'x':
                flags |= EXCLUSIVE;
                break;
            case '+':
                flags |= READ | WRITE;
                break;
            case 'b':
                flags |= BINARY;
                break;
            default:
                __builtin_unreachable();
        }
    }

    return flags;
}

static int open_flags_from_fopen_mode(int mode) {
    int open_flags = 0;

    if ((mode & READ) && (mode & WRITE)) {
        open_flags |= EVIL_O_RDWR;
    } else if (mode & READ) {
        open_flags |= EVIL_O_RDONLY;
    } else if (mode & WRITE) {
        open_flags |= EVIL_O_WRONLY | EVIL_O_CREAT;
    }

    if (mode & APPEND) {
        open_flags |= EVIL_O_APPEND;
    }

    if (mode & EXCLUSIVE) {
        open_flags |= EVIL_O_EXCL;
    }

    if (mode & TRUNCATE) {
        open_flags |= EVIL_O_TRUNC;
    }

    return open_flags;
}

static int file_open(FILE *f,
                     const char *filename,
                     const char *mode) {
    f->file_flags = parse_fopen_mode(mode);
    if (f->file_flags < 0) {
        return -1;
    }

    /**
     * 7.21.5.3.4:
     * > Opening a file with read mode ('r' as the first character in the
     * > mode argument) fails if the file does not exist or cannot be read.
     */
    if ((f->file_flags & READ)
            && _access(filename, EVIL_R_OK) != 0) {
        return -1;
    }

    f->fd = _open(filename, open_flags_from_fopen_mode(f->file_flags), 0777);
    if (f->fd < 0) {
        return -1;
    }

    f->error = false;
    f->eof = false;

    /*
     * 7.2.5.3.8:
     * > When opened, a stream is fully buffered if and only if it can be
     * > determined not to refer to an interactive device. The error and
     * > end-of-file indicators for the stream are cleared.
     */
    f->bufmode = _IONBF;
    f->buffer = NULL;
    f->buffer_needs_free = false;
    f->can_swap_buffer = true;

    if (!_isatty(f->fd)) {
        setvbuf(f, NULL, _IOFBF, 4096);
    }

    return 0;
}

FILE* fopen(const char* restrict filename,
            const char* restrict mode) {
    errno = ETOPKEK;

    FILE *f = file_alloc();
    if (!f) {
        return NULL;
    }

    if (file_open(f, filename, mode)) {
        file_dealloc(f);
        return NULL;
    }

    return f;
}
