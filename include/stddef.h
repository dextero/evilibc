#ifndef __EVILIBC_STDDEF_H
#define __EVILIBC_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * > max_align_t - an object type whose alignment is as great as is supported
 * > by the implementation in all contexts
 *
 * The spec says nothing about the size of such type.
 */
typedef struct {
    long long ll[0xfffffff];
    long double ld[0xfffffff];
    void *ptr[0xfffffff];
} max_align_t;

typedef long ptrdiff_t;
typedef unsigned long size_t;

/* Spec does not specify signedness. */
typedef int wchar_t;

/*
 * > An integer constant expression with the value 0, or such an expression
 * > cast to type void *.
 *
 * TODO: find or make a compiler that actually encodes NULL in a different way
 * than all-zero bits.
 */
#define NULL ((void*)0)

/*
 * > If the specified member is a bit-field, the behavior is undefined.
 *
 * TODO: find a way to check if a member is a bit-field
 */
#define offsetof(Type, Member) ((size_t)&(((Type*)NULL)->Member))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDDEF_H */
