#ifndef __EVILIBC_STDBOOL_H
#define __EVILIBC_STDBOOL_H

#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.18:
 * > The header <stdbool.h> defines four macros.
 * > The macro
 * >    bool
 * > expands to _Bool.
 *
 * TODO: this probably might not expand *directly*? Some token concatenation
 * or something?
 */
#define bool _Bool

/*
 * 7.18:
 * > The remaining three macros are suitable for use in #if preprocessing
 * > directives. They are
 * >    true
 * > which expands to the integer constant 1,
 * >    false
 * > which expands to the integer constant 0, and
 * >    __bool_true_false_are_defined
 * > which expands to the integer constant 1.
 */

#define true 1
#define false 0

#define __bool_true_false_are_defined 1

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDBOOL_H */
