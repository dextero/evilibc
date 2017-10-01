#ifndef __EVILIBC_OS_SYSCALLS_H
#define __EVILIBC_OS_SYSCALLS_H

#ifndef _NO_INCLUDES
# include <sys/types.h>
# include <sys/fcntl.h>
#endif // _NO_INCLUDES

#include <stddef.h>

typedef long ssize_t;

int _open(const char *path, int flags, int mode);
ssize_t _write(int fd, const void *buf, size_t count);
int _access(const char *path, int mode);
int _isatty(int fd);
void _exit(int exit_code) __attribute__((noreturn));
int _kill(long pid, int sig);
long _getpid(void);


#endif // __EVILIBC_OS_SYSCALLS_H
