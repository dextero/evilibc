#define _NO_INCLUDES
#include "os/syscalls.h"

#include "stdarg.h"

#include <sys/syscall.h>

#include <sys/ioctl.h>
#include <sys/termios.h>

#define EXPAND(A) A
#define _CONCAT(A, B) A##B
#define CONCAT(A, B) _CONCAT(A, B)

#define _NARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define NARGS(...) EXPAND(_NARGS(_, ##__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define _EXPAND_N(F, N, ...) EXPAND(CONCAT(F, N)(__VA_ARGS__))
#define EXPAND_N(F, ...) _EXPAND_N(F, NARGS(__VA_ARGS__), ##__VA_ARGS__)

#define ARGS_0()                 void
#define ARGS_2(Type, Name)       Type Name
#define ARGS_4(Type, Name, ...)  Type Name, ARGS_2(__VA_ARGS__)
#define ARGS_6(Type, Name, ...)  Type Name, ARGS_4(__VA_ARGS__)
#define ARGS_8(Type, Name, ...)  Type Name, ARGS_6(__VA_ARGS__)
#define ARGS_10(Type, Name, ...) Type Name, ARGS_8(__VA_ARGS__)
#define ARGS_12(Type, Name, ...) Type Name, ARGS_10(__VA_ARGS__)
#define ARGS(...) EXPAND_N(ARGS_, ##__VA_ARGS__)

#define SETUP_0()
#define SETUP_2(Type, Name)                             "movq %[arg0], %%rdi\n"
#define SETUP_4(Type, Name, ...)  SETUP_2(__VA_ARGS__)  "movq %[arg1], %%rsi\n"
#define SETUP_6(Type, Name, ...)  SETUP_4(__VA_ARGS__)  "movq %[arg2], %%rdx\n"
#define SETUP_8(Type, Name, ...)  SETUP_6(__VA_ARGS__)  "movq %[arg3], %%r10\n"
#define SETUP_10(Type, Name, ...) SETUP_8(__VA_ARGS__)  "movq %[arg4], %%r8\n"
#define SETUP_12(Type, Name, ...) SETUP_10(__VA_ARGS__) "movq %[arg5], %%r9\n"
#define SETUP(...) EXPAND_N(SETUP_, ##__VA_ARGS__)

#define REVERSE_NAMES_0()
#define REVERSE_NAMES_2(Type, Name, ...)                                 Name
#define REVERSE_NAMES_4(Type, Name, ...)  REVERSE_NAMES_2(__VA_ARGS__),  Name
#define REVERSE_NAMES_6(Type, Name, ...)  REVERSE_NAMES_4(__VA_ARGS__),  Name
#define REVERSE_NAMES_8(Type, Name, ...)  REVERSE_NAMES_6(__VA_ARGS__),  Name
#define REVERSE_NAMES_10(Type, Name, ...) REVERSE_NAMES_8(__VA_ARGS__),  Name
#define REVERSE_NAMES_12(Type, Name, ...) REVERSE_NAMES_10(__VA_ARGS__), Name
#define REVERSE_NAMES(...) EXPAND_N(REVERSE_NAMES_, ##__VA_ARGS__)

#define INPUTS_0()
#define INPUTS_1(Name)                           , [arg0]"imr"((unsigned long)Name)
#define INPUTS_2(Name, ...) INPUTS_1(__VA_ARGS__), [arg1]"imr"((unsigned long)Name)
#define INPUTS_3(Name, ...) INPUTS_2(__VA_ARGS__), [arg2]"imr"((unsigned long)Name)
#define INPUTS_4(Name, ...) INPUTS_3(__VA_ARGS__), [arg3]"imr"((unsigned long)Name)
#define INPUTS_5(Name, ...) INPUTS_4(__VA_ARGS__), [arg4]"imr"((unsigned long)Name)
#define INPUTS_6(Name, ...) INPUTS_5(__VA_ARGS__), [arg5]"imr"((unsigned long)Name)

// TODO: A *terrible* hack. I'm not sure why can't this just work with EXPAND_N.
#define _EXPAND_N_2(F, N, ...) EXPAND(CONCAT(F, N)(__VA_ARGS__))
#define EXPAND_N_2(F, ...) _EXPAND_N_2(F, NARGS(__VA_ARGS__), ##__VA_ARGS__)

#define INPUTS(...) EXPAND_N_2(INPUTS_, REVERSE_NAMES(__VA_ARGS__))

#define CLOBBERS_0()
#define CLOBBERS_2(Type, Name)                              ,  "rdi"
#define CLOBBERS_4(Type, Name, ...)  CLOBBERS_2(__VA_ARGS__),  "rsi"
#define CLOBBERS_6(Type, Name, ...)  CLOBBERS_4(__VA_ARGS__),  "rdx"
#define CLOBBERS_8(Type, Name, ...)  CLOBBERS_6(__VA_ARGS__),  "r10"
#define CLOBBERS_10(Type, Name, ...) CLOBBERS_8(__VA_ARGS__),  "r8"
#define CLOBBERS_12(Type, Name, ...) CLOBBERS_10(__VA_ARGS__), "r9"
#define CLOBBERS(...) EXPAND_N(CLOBBERS_, ##__VA_ARGS__)

#define RETURN_void(Name) (void)Name
#define RETURN_long(Name) return Name
#define RETURN_ssize_t(Name) RETURN_long(Name)
#define RETURN_int(Name) return (int)Name
#define RETURN(Type, Name) EXPAND(CONCAT(RETURN_, Type)(Name))

#define SYSCALL(Num, RetT, Name, ...)                   \
    RetT Name(ARGS(__VA_ARGS__)) {                      \
        long result;                                    \
        asm("movq %[syscall], %%rax\n"                  \
            SETUP(__VA_ARGS__)                          \
            "syscall\n"                                 \
            "movq %%rax, %[result]\n"                   \
            : [result]"=r"(result)                      \
            : [syscall]"i"(Num) INPUTS(__VA_ARGS__)     \
            : "rax" CLOBBERS(__VA_ARGS__));             \
        return (RetT)result;                            \
    }

SYSCALL(__NR_open, int, _open, const char *, path, int, flags, int, mode)
SYSCALL(__NR_access, int, _access, const char *, path, int, flags)
SYSCALL(__NR_write, ssize_t, _write, int, fd, const void *, buf, size_t, count)
SYSCALL(__NR_kill, int, _kill, long, pid, int, sig)
SYSCALL(__NR_getpid, long, _getpid)

int _isatty(int fd) {
    // TODO: ??? glibc seems to do this
    struct termios termios;
    return ioctl(fd, TCGETS, &termios) == 0;
}

static SYSCALL(__NR_exit, void, sys_exit, int, exit_code)

void _exit(int exit_code) {
    sys_exit(exit_code);
    __builtin_unreachable();
}

static SYSCALL(__NR_ioctl, long, sys_ioctl,
               int, fd, unsigned long, command, void *, arg)

int ioctl(int fd, unsigned long command, ...) {
    va_list list;
    va_start(list, command);
    int result = sys_ioctl(fd, command, va_arg(list, void*));
    va_end(list);
    return result;
}

static SYSCALL(__NR_brk, void *, _brk, void *, newbrk)

void *_sbrk(ptrdiff_t increment) {
    static void *curr_brk = NULL;
    if (!curr_brk) {
        curr_brk = _brk(NULL);
    }

    void *result = curr_brk;
    curr_brk = _brk((char *)curr_brk + increment);

    if (curr_brk == result) {
        return NULL;
    } else {
        return curr_brk;
    }
}
