#ifndef __EVILIBC_ASSERT_H
#define __EVILIBC_ASSERT_H

#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * NOTE: these are amazingly well-defined. One could print rainbow-colored
 * error messages or something, but I doubt breaking it in any interesting
 * way is even possible.
 */

/*
 * 7.2:
 * > The assert macro is redefined according to the current state of NDEBUG
 * > each time that <assert.h> is included.
 */
#undef assert

#ifdef NDEBUG

/**
 * 7.2:
 * > If NDEBUG is defined as a macro name at the point in the source file where
 * > <assert.h> is included, the assert macro is defined simply as
 * >    #define assert(ignore) ((void)0)
 */
#define assert(ignore) ((void)0)

#else /* NDEBUG */

/*
 * 7.2.1.1:
 * > The assert macro puts diagnostic tests into programs; it expands to a void
 * > expression. When it is executed, if expression (which shall have a scalar
 * > type) is false (that is, compares equal to 0), the assert macro writes
 * > information about the particular call that failed (including the text of
 * > the argument, the name of the source file, the source line number, and the
 * > name of the enclosing function -- the latter are respectively the values
 * > of the preprocessing macros __FILE__ and __LINE__ and of the identifier
 * > __func__) on the standard error stream in an implementation-defined format.
 * > It then calls the abort function.
 *
 * 7.2.1.1:
 * > The assert macro returns no value.
 */

#define __EVILIBC_QUOTE_(x) #x
#define __EVILIBC_QUOTE(x) __EVILIBC_QUOTE_(x)

#define assert(expression) \
    do { \
        if ((expression) == 0) { \
            fprintf(stderr, "Assertion failed: " #expression ", function %s (" \
                    __FILE__ ":" __EVILIBC_QUOTE(__LINE__) ")\n", __func__); \
            abort(); \
        } \
    } while (0)
#endif /* NDEBUG */

/*
 * 7.2:
 * > The macro
 * >    static_assert
 * > expands to _Static_assert.
 */
#define static_assert _Static_assert

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_ASSERT_H */
