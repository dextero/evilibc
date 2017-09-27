#define _NO_INCLUDES
#include "os/syscalls.h"

#include "stdarg.h"

#include <sys/syscall.h>

#include <sys/ioctl.h>
#include <sys/termios.h>

#define EXPAND(A) A
#define _CONCAT(A, B) A##B
#define CONCAT(A, B) _CONCAT(A, B)

#define _EXPAND_N(F, N, ...) EXPAND(CONCAT(F, N)(__VA_ARGS__))
#define EXPAND_N(F, ...) _EXPAND_N(F, NARGS(__VA_ARGS__), __VA_ARGS__)

#define ARGS_2(Type, Name) Type Name
#define ARGS_4(Type, Name, ...) Type Name, ARGS_2(__VA_ARGS__)
#define ARGS_6(Type, Name, ...) Type Name, ARGS_4(__VA_ARGS__)
#define ARGS_8(Type, Name, ...) Type Name, ARGS_6(__VA_ARGS__)
#define ARGS_10(Type, Name, ...) Type Name, ARGS_8(__VA_ARGS__)
#define ARGS_12(Type, Name, ...) Type Name, ARGS_10(__VA_ARGS__)
#define ARGS(...) EXPAND_N(ARGS_, __VA_ARGS__)

#define SETUP_2(Type, Name) "movq %2, %%rdi\n"
#define SETUP_4(Type, Name, ...) "movq %3, %%rsi\n" SETUP_2(__VA_ARGS__)
#define SETUP_6(Type, Name, ...) "movq %4, %%rdx\n" SETUP_4(__VA_ARGS__)
#define SETUP_8(Type, Name, ...) "movq %5, %%r10\n" SETUP_6(__VA_ARGS__)
#define SETUP_10(Type, Name, ...) "movq %6, %%r8\n" SETUP_8(__VA_ARGS__)
#define SETUP_12(Type, Name, ...) "movq %7, %%r9\n" SETUP_10(__VA_ARGS__)
#define SETUP(...) EXPAND_N(SETUP_, __VA_ARGS__)

#define INPUTS_2(Type, Name) "imr"(Name)
#define INPUTS_4(Type, Name, ...) "imr"(Name), INPUTS_2(__VA_ARGS__)
#define INPUTS_6(Type, Name, ...) "imr"(Name), INPUTS_4(__VA_ARGS__)
#define INPUTS_8(Type, Name, ...) "imr"(Name), INPUTS_6(__VA_ARGS__)
#define INPUTS_10(Type, Name, ...) "imr"(Name), INPUTS_8(__VA_ARGS__)
#define INPUTS_12(Type, Name, ...) "imr"(Name), INPUTS_10(__VA_ARGS__)
#define INPUTS(...) EXPAND_N(INPUTS_, __VA_ARGS__)

#define CLOBBERS_2(Type, Name) "rdi"
#define CLOBBERS_4(Type, Name, ...) "rsi", CLOBBERS_2(__VA_ARGS__)
#define CLOBBERS_6(Type, Name, ...) "rdx", CLOBBERS_4(__VA_ARGS__)
#define CLOBBERS_8(Type, Name, ...) "r10", CLOBBERS_6(__VA_ARGS__)
#define CLOBBERS_10(Type, Name, ...) "r8", CLOBBERS_8(__VA_ARGS__)
#define CLOBBERS_12(Type, Name, ...) "r9", CLOBBERS_10(__VA_ARGS__)
#define CLOBBERS(...) EXPAND_N(CLOBBERS_, __VA_ARGS__)

#define _NARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define NARGS(...) EXPAND(_NARGS(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define DECLARE_void(Name)
#define DECLARE_long(Name) long Name;
#define DECLARE(Type, Name) EXPAND(CONCAT(DECLARE_, Type)(Name))

#define ASSIGN_void(Name)
#define ASSIGN_long(Name) "=r"(Name)
#define ASSIGN(Type, Name) EXPAND(CONCAT(ASSIGN_, Type)(Name))

#define RETURN_void(Name)
#define RETURN_long(Name) return Name;
#define RETURN(Type, Name) EXPAND(CONCAT(RETURN_, Type)(Name))

#define SYSCALL(Num, RetT, Name, ...) \
    static RetT Name(ARGS(__VA_ARGS__) ) {      \
        long result;                            \
        asm("movq %1, %%rax\n"                  \
            SETUP(__VA_ARGS__)                  \
            "syscall\n"                         \
            "movq %%rax, %0\n"                  \
            : "=r"(result)                      \
            : "i"(Num), INPUTS(__VA_ARGS__)     \
            : "rax", CLOBBERS(__VA_ARGS__));    \
            RETURN(RetT, result)                \
    }

SYSCALL(__NR_open, long, sys_open, const char *, path, int, flags)

int _open(const char *path, int flags, ...) {
    return sys_open(path, flags);
}

SYSCALL(__NR_access, long, sys_access, const char *, path, int, flags)

int _access(const char *path, int flags) {
    return sys_access(path, flags);
}

int _isatty(int fd) {
    // TODO: ??? glibc seems to do this
    struct termios termios;
    return ioctl(fd, TCGETS, &termios) == 0;
}

SYSCALL(__NR_exit, void, sys_exit, int, exit_code)

void _exit(int exit_code) {
    sys_exit(exit_code);
    __builtin_unreachable();
}

SYSCALL(__NR_ioctl, long, sys_ioctl, int, fd, unsigned long, command, void *, arg)

int ioctl(int fd, unsigned long command, ...) {
    va_list list;
    va_start(list, command);
    int result = sys_ioctl(fd, command, va_arg(list, void*));
    va_end(list);
    return result;
}
