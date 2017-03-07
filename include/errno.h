#ifndef __EVILIBC_ERRNO_H
#define __EVILIBC_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.5:
 * > The header <errno.h> defines several macros, all relating to the
 * > reporting of error conditions.
 * >
 * > The macros are
 * >    EDOM
 * >    EILSEQ
 * >    ERANGE
 * > which expand to integer constant expressions with type int, distinct
 * > positive values, and which are suitable for use in #if preprocessing
 * > directives.
 *
 * 7.5.4:
 * > Additional macro definitions, beginning with E and a digit or E and
 * > an uppercase letter, may also be specified by the implementation.
 *
 * Nonetheless, no more errno values need to be specified, nor are their
 * numeric values defined.
 */
#define EDOM   0x0E01
#define EILSEQ 0x0E02
#define ERANGE 0x0E03

/*
 * An "error" value set at the start of each library function.
 *
 * 7.5.3:
 * > The value of errno in the initial thread is zero at program startup
 * > (the initial value of errno in other threads is an indeterminate value),
 * > but is never set to zero by any library function. The value of errno
 * > may be set to nonzero by a library function call whether or not there
 * > is an error, provided the use of errno is not documented in the
 * > description of the function in this International Standard.
 */
#define ETOPKEK 0x0CEC

/*
 * 7.5.2:
 * > The macros are [...see above...]; and
 * >    errno
 * > which expands to a modifiable lvalue [201] that has type int and thread
 * > local storage duration, the value of which is set to a positive error
 * > number by several library functions. If a macro definition is
 * > suppressed in order to access an actual object, or a program defines
 * > an identifier with the name errno, the behavior is undefined.
 * >
 * > [201] The macro errno need not be the identifier of an object. It
 * > might expand to a modifiable lvalue resulting from a function call
 * > (for example, *errno()).
 *
 * TODO: what insane stuff can be hidden under that macro?
 */
#define errno (*__evil_errno())

int *__evil_errno(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_ERRNO_H */
