#include "SyscallsMock.h"

thread_local evil::SyscallsMock* evil::SyscallsMock::instance;

#define EXPAND(A) A
#define _CONCAT(A, B) A##B
#define CONCAT(A, B) _CONCAT(A, B)

#define _NARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define NARGS(...) EXPAND(_NARGS(_, ##__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define _EXPAND_N(F, N, ...) EXPAND(CONCAT(F, N)(__VA_ARGS__))
#define EXPAND_N(F, ...) _EXPAND_N(F, NARGS(__VA_ARGS__), ##__VA_ARGS__)

#define _ARG_LIST_0() void
#define _ARG_LIST_2(T, N)       T N
#define _ARG_LIST_4(T, N, ...)  T N, _ARG_LIST_2(__VA_ARGS__)
#define _ARG_LIST_6(T, N, ...)  T N, _ARG_LIST_4(__VA_ARGS__)
#define _ARG_LIST_8(T, N, ...)  T N, _ARG_LIST_6(__VA_ARGS__)
#define _ARG_LIST_10(T, N, ...) T N, _ARG_LIST_8(__VA_ARGS__)
#define _ARG_LIST_12(T, N, ...) T N, _ARG_LIST_10(__VA_ARGS__)
#define _ARG_LIST(...) EXPAND_N(_ARG_LIST_, ##__VA_ARGS__)

#define _ARG_CALL_0()
#define _ARG_CALL_2(_, N)       N
#define _ARG_CALL_4(_, N, ...)  N, _ARG_CALL_2(__VA_ARGS__)
#define _ARG_CALL_6(_, N, ...)  N, _ARG_CALL_4(__VA_ARGS__)
#define _ARG_CALL_8(_, N, ...)  N, _ARG_CALL_6(__VA_ARGS__)
#define _ARG_CALL_10(_, N, ...) N, _ARG_CALL_8(__VA_ARGS__)
#define _ARG_CALL_12(_, N, ...) N, _ARG_CALL_10(__VA_ARGS__)
#define _ARG_CALL(...) EXPAND_N(_ARG_CALL_, ##__VA_ARGS__)

#define SYSCALL(RetT, Name, ...) \
    RetT Name(_ARG_LIST(__VA_ARGS__)) { \
        return ::evil::SyscallsMock::instance->Name(_ARG_CALL(__VA_ARGS__)); \
    }

extern "C" {

SYSCALL(int, _access, const char *, path, int, mode);
SYSCALL(int, _close, int, fd);
SYSCALL(int, _isatty, int, fd);
SYSCALL(int, _kill, long, pid, int, sig);
SYSCALL(int, _open, const char *, path, int, flags, int, mode);
SYSCALL(long, _getpid);
SYSCALL(ssize_t, _write, int, fd, const void *, buf, size_t, count);
SYSCALL(void *, _sbrk, ptrdiff_t, increment);

void _exit(int code) {
    ::evil::SyscallsMock::instance->_exit(code);
    __builtin_unreachable();
}

} // extern "C"
