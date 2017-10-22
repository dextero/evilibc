#include "os/syscalls.h"

#include "stdarg.h"

int __attribute__((weak)) _access(const char *path, int flags) {
    (void) path;
    (void) flags;
    return -1;
}

int __attribute__((weak)) _close(int fd) {
    (void) fd;
    return -1;
}

long __attribute__((weak)) _getpid(void) {
    return -1;
}

int __attribute__((weak)) _kill(long pid, int sig) {
    (void) pid;
    (void) sig;
    return -1;
}

int __attribute__((weak)) _open(const char *path, int flags, int mode) {
    (void) path;
    (void) flags;
    (void) mode;
    return -1;
}

ssize_t __attribute__((weak)) _write(int fd, const void * buf, size_t count) {
    (void) fd;
    (void) buf;
    (void) count;
    return -1;
}

int __attribute__((weak)) _isatty(int fd) {
    (void) fd;
    return -1;
}

void __attribute__((weak)) _exit(int exit_code) {
    (void) exit_code;
    for (;;) {}
    __builtin_unreachable();
}

void *__attribute__((weak)) _sbrk(ptrdiff_t increment) {
    (void) increment;
    return NULL;
}
