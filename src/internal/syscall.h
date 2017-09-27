#ifndef __EVILIBC_INTERNAL_SYSCALL_H
#define __EVILIBC_INTERNAL_SYSCALL_H

#include <sys/syscall.h>

#define SYSCALL_2(Num, Name,                                        \
                  Type1, ArgName1,                                  \
                  Type2, ArgName2)                                  \
    long sys_##Name(Type1 ArgName1,                                 \
                    Type2 ArgName2) {                               \
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
    long sys_##Name(Type1 ArgName1,                                 \
                    Type2 ArgName2,                                 \
                    Type3 ArgName3) {                               \
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
SYSCALL_2(__NR_access, access, const char *, path, int, flags)

#endif /* __EVILIBC_INTERNAL_SYSCALL_H */
