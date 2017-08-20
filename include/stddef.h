#ifndef __EVILIBC_STDDEF_H
#define __EVILIBC_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.19.2:
 * >    max_align_t
 * > which is an object type whose alignment is as great as is supported
 * > by the implementation in all contexts
 *
 * Well, the *size* of this type is not defined, so...
 */
typedef struct {
    long long ll[0xfffffff];
    long double ld[0xfffffff];
    void* ptr[0xfffffff];
} max_align_t;

/*
 * 7.19.2:
 * >    ptrdiff_t
 * > which is the signed integer type of the result of subtracting two pointers
 */
typedef __PTRDIFF_TYPE__ ptrdiff_t;

/*
 * 7.19.2:
 * >    size_t
 * > which is the unsigned integer type of the result of the sizeof operator
 */
typedef __SIZE_TYPE__ size_t;

/*
 * 7.19.2:
 * >    wchar_t
 * > which is an integer type whose range of values can represent distinct
 * > codes for all members of the largest extended character set specified
 * > among the supported locales; the null character shall have the code
 * > value zero. Each member of the basic character set shall have a code
 * > value equal to its value when used as the lone character in an integer
 * > character constant if an implementation does not define
 * > __STDC_MB_MIGHT_NEQ_WC__.
 *
 * TODO: __STDC_MB_MIGHT_NEQ_WC__ looks promising. It would require
 * a particularly evil compiler though.
 */
typedef int wchar_t;

/*
 * > 7.19.3:
 * > The macros are
 * >    NULL
 * > which expands to an implementation-defined null pointer constant;
 *
 * 6.3.2.3:
 * > An integer constant expression with the value 0, or such an expression
 * > cast to type void *, is called a *null pointer constant*.
 *
 * TODO: find or make a compiler that actually encodes NULL in a different way
 * than all-zero bits.
 */
#define NULL ((void*)0)

/*
 * 7.19.3:
 * > The macros are [...]:
 * >    offsetof(type, member-designator)
 * > which expands to an integer constant expression that has type size_t,
 * > the value of which is the offset in bytes, to the structure member
 * > (designated by member-designator), from the beginning of its structure
 * > (designated by type). The type and member designator shall be such that
 * > given
 * >    static type t;
 * > then the expression &(t.member-designator) evaluates to an address
 * > constant. (If the specified member is a bit-field, the behavior is
 * > undefined.)
 *
 * TODO: find a way to check if a member is a bit-field and trigger UB
 */
#define offsetof __builtin_offsetof

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDDEF_H */
