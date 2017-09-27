#define _NO_INCLUDES
#include "os/syscalls.h"

#include <sys/syscall.h>

#include <sys/ioctl.h>
#include <sys/termios.h>

#define SYSCALL_2(Num, Name,                                        \
                  Type1, ArgName1,                                  \
                  Type2, ArgName2)                                  \
    static long Name(Type1 ArgName1,                                \
                     Type2 ArgName2) {                              \
        long result;                                                \
        asm("movq %0, %%rax\n"                                      \
            "movq %1, %%rdi\n"                                      \
            "movq %2, %%rsi\n"                                      \
            "syscall\n"                                             \
            "movq %%rax, %3\n"                                      \
            : "=r"(result)                                          \
            : "i"(Num), "imr" (ArgName1), "imr" (ArgName2)          \
            : "rax", "rdi", "rsi");                                 \
        return result;                                              \
    }

#define SYSCALL_3(Num, Name,                                        \
                  Type1, ArgName1,                                  \
                  Type2, ArgName2,                                  \
                  Type3, ArgName3)                                  \
    static long Name(Type1 ArgName1,                                \
                     Type2 ArgName2,                                \
                     Type3 ArgName3) {                              \
        asm("movq %0, %%rax\n"                                      \
            "movq %1, %%rdi\n"                                      \
            "movq %2, %%rsi\n"                                      \
            "movq %3, %%rdx\n"                                      \
            "syscall\n"                                             \
            "movq %%rax, %4\n"                                      \
            : "=r"(result)                                          \
            : "i"(Num), "imr" (ArgName1), "imr" (ArgName2),         \
              "imr"(ArgName3)                                       \
            : "rax", "rdi", "rdx", "rsi");                          \
        return result;                                              \
    }

SYSCALL_2(__NR_open, open, const char *, path, int, flags)

int _open(const char *path, int flags, ...) {
    return open(path, flags);
}

SYSCALL_2(__NR_access, access, const char *, path, int, flags)

int _access(const char *path, int flags) {
    return access(path, flags);
}

int _isatty(int fd) {
    // TODO: ??? glibc seems to do this
    struct termios termios;
    return ioctl(fd, TCGETS, &termios) == 0;
}

