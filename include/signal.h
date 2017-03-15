#ifndef __EVILIBC_SIGNAL_H
#define __EVILIBC_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.14:
 * > The type defined is
 * >    sig_atomic_t
 * > which is the (possibly volatile-qualified) integer type of an object that
 * > can be accessed as an atomic entity, even in the presence of asynchronous
 * > interrupts.
 *
 * 7.20.3.3:
 * > If sig_atomic_t (see 7.14) is defined as a signed integer type, the
 * > value of SIG_ATOMIC_MIN shall be no greater than −127 and the value
 * > of SIG_ATOMIC_MAX shall be no less than 127; otherwise, sig_atomic_t
 * > is defined as an unsigned integer type, and the value of SIG_ATOMIC_MIN
 * > shall be 0 and the value of SIG_ATOMIC_MAX shall be no less than 255.
 *
 * That means char is big enough.
 */
typedef char sig_atomic_t;

/*
 * 7.14:
 * > The macros defined are
 * >    SIG_DFL
 * >    SIG_ERR
 * >    SIG_IGN
 * > which expand to constant expressions with distinct values that have type
 * > compatible with the second argument to, and the return value of, the
 * > signal function, and whose values compare unequal to the address of any
 * > declarable function;
 *
 * The last bit is a bit problematic. I'd really like to use some arbitrary
 * values that happen to be forbidden for functions, but it doesn't seem like
 * functions need to be aligned in any particular way.
 *
 * Setting them to pointers to some pre-defined functions may also not work
 * if linkers decide they look similar to user-defined ones and merge them.
 *
 * The only way of defining them I can see is leveraging the fact that a first
 * memory page is inaccessible on Linux. TODO?
 */ 
#define SIG_DFL 0x0
#define SIG_ERR 0x1
#define SIG_IGN 0x2

/*
 * 7.14.4:
 * > [...] and the following, which expand to positive integer constant
 * > expressions with type int and distinct values that are the signal numbers,
 * > each corresponding to the specified condition:
 * >    SIGABRT abnormal termination, such as is initiated by the abort function
 * >    SIGFPE  an erroneous arithmetic operation, such as zero divide or an
 * >            operation resulting in overflow
 * >    SIGILL  detection of an invalid function image, such as an invalid
 * >            instruction
 * >    SIGINT  receipt of an interactive attention signal
 * >    SIGSEGV an invalid access to storage
 * >    SIGTERM a termination request sent to the program
 * >
 * > [...] all signal numbers shall be positive.
 *
 * Even though it could be amusing to use arbitrary signal values, ones used
 * here are either POSIX- or Linux-compatible.
 */

#define SIGABRT 6
#define SIGFPE 8
#define SIGILL 4
#define SIGINT 2
#define SIGSEGV 11
#define SIGTERM 15

/*
 * Equivalent to:
 *     typedef void sig_handler_t(int);
 *     sig_handler_t* signal(int sig, sig_handler_t* func);
 *
 * Here defined as in the C spec: in its most basic, disgusting form.
 */
void (*signal(int sig, void (*func)(int)))(int);  

int raise(int sig);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_SIGNAL_H */
