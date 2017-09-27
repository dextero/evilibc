#ifndef __EVILIBC_OS_SYSCALLS_H
#define __EVILIBC_OS_SYSCALLS_H

#ifndef _NO_INCLUDES
# include <sys/types.h>
# include <sys/fcntl.h>
#endif // _NO_INCLUDES

int _open(const char *path, int mode, ...);
int _access(const char *path, int mode);
int _isatty(int fd);

#endif // __EVILIBC_OS_SYSCALLS_H
