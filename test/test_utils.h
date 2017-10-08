#ifndef __EVILIBC_TEST_UTILS_H
#define __EVILIBC_TEST_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#define __evil_set_ub_handler test___evil_set_ub_handler
#define __evil_set_idb_handler test___evil_set_idb_handler
#include "../include/evilibc.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_TEST_UTILS_H */
