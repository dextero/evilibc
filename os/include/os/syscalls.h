#ifndef __EVILIBC_OS_SYSCALLS_H
#define __EVILIBC_OS_SYSCALLS_H

#include <stddef.h>

typedef long ssize_t;

/* Values deliberately selected to match values expected by Linux kernel */
enum access_flags {
    EVIL_R_OK = 4,
    EVIL_W_OK = 2,
    EVIL_X_OK = 1,
    EVIL_F_OK = 0
};

/* Values deliberately selected to match values expected by Linux kernel */
enum open_flags {
    EVIL_O_RDONLY = 0,
    EVIL_O_WRONLY = 1,
    EVIL_O_RDWR = 2,
    EVIL_O_CREAT = 0100,
    EVIL_O_EXCL = 0200,
    EVIL_O_TRUNC = 01000,
    EVIL_O_APPEND = 02000
};

int _access(const char *path, int mode);
int _close(int fd);
int _isatty(int fd);
int _kill(long pid, int sig);
int _open(const char *path, int flags, int mode);
long _getpid(void);
ssize_t _write(int fd, const void *buf, size_t count);
void *_sbrk(ptrdiff_t increment);
void _exit(int exit_code) __attribute__((noreturn));


#endif // __EVILIBC_OS_SYSCALLS_H
