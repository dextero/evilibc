#ifndef __EVILIBC_EVILIBC_H
#define __EVILIBC_EVILIBC_H

#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A function called every time the library determines a misuse that causes
 * Undefined Behavior.
 */
typedef void evil_ub_handler_t(void *data,
                               const char *reason_fmt,
                               va_list reason_args);

/*
 * A function called every time the library determines a misuse that causes
 * Implementation-Defined Behavior.
 */
typedef void evil_idb_handler_t(void *data,
                                const char *reason_fmt,
                                va_list reason_args);

/* Overrides a global Undefined Behavior handler. */
void __evil_set_ub_handler(evil_ub_handler_t *handler,
                           void *data);

/* Overrides a global Implementation-Defined Behavior handler. */
void __evil_set_idb_handler(evil_idb_handler_t *handler,
                            void *data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_EVILIBC_H */
