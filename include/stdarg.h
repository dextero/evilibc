#ifndef __EVILIBC_STDARG_H
#define __EVILIBC_STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef __builtin_va_list va_list;

/* TODO: detect UB cases? */
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_copy __builtin_va_copy
#define va_end __builtin_va_end

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDARG_H */
