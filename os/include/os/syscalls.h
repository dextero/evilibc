#ifndef __EVILIBC_OS_SYSCALLS_H
#define __EVILIBC_OS_SYSCALLS_H

#ifndef _NO_INCLUDES
# include <sys/types.h>
# include <sys/fcntl.h>
#endif // _NO_INCLUDES

#include <stddef.h>

typedef long ssize_t;

int _access(const char *path, int mode);
void _exit(int exit_code) __attribute__((noreturn));
long _getpid(void);
int _isatty(int fd);
int _kill(long pid, int sig);
int _open(const char *path, int flags, int mode);
void *_sbrk(ptrdiff_t increment);
ssize_t _write(int fd, const void *buf, size_t count);


#endif // __EVILIBC_OS_SYSCALLS_H
