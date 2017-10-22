#ifndef __EVIL_TEST_SYSCALLS_MOCK_H
#define __EVIL_TEST_SYSCALLS_MOCK_H

#include <gmock/gmock.h>

#include "os/syscalls.h"

namespace evil {

struct SyscallsMock
{
    static thread_local SyscallsMock *instance;
    SyscallsMock() { instance = this; }
    ~SyscallsMock() { instance = nullptr; }

    MOCK_METHOD2(_access, int(const char *path, int mode));
    MOCK_METHOD1(_close,  int(int fd));
    MOCK_METHOD1(_isatty, int(int fd));
    MOCK_METHOD2(_kill,   int(long pid, int sig));
    MOCK_METHOD3(_open,   int(const char *path, int flags, int mode));
    MOCK_METHOD0(_getpid, long(void));
    MOCK_METHOD3(_write,  ssize_t(int fd, const void *buf, size_t count));
    MOCK_METHOD1(_sbrk,   void *(ptrdiff_t increment));
    MOCK_METHOD1(_exit,   void(int exit_code));
};

} // namespace evil

#endif // __EVIL_TEST_SYSCALLS_MOCK_H
