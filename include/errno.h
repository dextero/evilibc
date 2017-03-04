#ifndef __EVILIBC_ERRNO_H
#define __EVILIBC_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Required to be distinct positive ints */
#define EDOM   0xE000001
#define EILSEQ 0xE000002
#define ERANGE 0xE000003

/* Required to expand to a modifiable lvalue of type int */
#define errno (*__evil_errno())

int *__evil_errno(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_ERRNO_H */
