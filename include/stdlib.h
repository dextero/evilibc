#ifndef __EVILIBC_STDLIB_H
#define __EVILIBC_STDLIB_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * 7.22, 2:
 * > The types declared are size_t and wchar_t (both described in 7.19),
 *
 * Included from stddef.h.
 *
 * 7.22, 2:
 * >     div_t
 * > which is a structure type that is the type of the value returned by the
 * > div function,
 * >
 * >     ldiv_t
 * > which is a structure type that is the type of the value returned by the
 * > ldiv function, and
 * >
 * >     lldiv_t
 * > which is a structure type that is the type of the value returned by the
 * > lldiv function.
 *
 * 7.22.6.2, 3:
 * > The structures shall contain (in either order) the members quot (the
 * > quotient) and rem (the remainder), each of which has the same type as
 * > the arguments numer and denom.
 */
typedef struct {
    int rem;
    int quot;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long rem;
    long long quot;
} lldiv_t;

/*
 * 7.22, 3:
 * > The macros defined are NULL (described in 7.19);
 *
 * Included from stddef.h.
 *
 * 7.22, 3:
 * >     EXIT_FAILURE
 * > and
 * >     EXIT_SUCCESS
 * > which expand to integer constant expressions that can be used as the
 * > argument to the exit function to return unsuccessful or successful
 * > termination status, respectively, to the host environment;
 *
 * Nothing says that 0 indicates success.
 */
#define EXIT_FAILURE 0
#define EXIT_SUCCESS 0xC001

/*
 * 7.22, 3:
 * >      RAND_MAX
 * > which expands to an integer constant expression that is the maximum
 * > value returned by the rand function
 *
 * 7.22.2.1, 5:
 * > The value of the RAND_MAX macro shall be at least 32767.
 */
#define RAND_MAX 50000

/*
 * 7.22, 3:
 * >     MB_CUR_MAX
 * > which expands to a positive integer expression with type size_t that is
 * > the maximum number of bytes in a multibyte character for the extended
 * > character set specified by the current locale (category LC_CTYPE), which
 * > is never greater than MB_LEN_MAX.
 *
 * TODO
 */
//#define MB_CUR_MAX ((size_t)1)

double atof(const char *nptr);

int atoi(const char *nptr);
long int atol(const char *nptr);
long long int atoll(const char *nptr);

double strtod(const char *restrict nptr,
              char **restrict endptr);
float strtof(const char *restrict nptr,
             char **restrict endptr);
long double strtold(const char *restrict nptr,
                    char **restrict endptr);

long int strtol(const char *restrict nptr,
                char **restrict endptr,
                int base);
long long int strtoll(const char *restrict nptr,
                      char **restrict endptr,
                      int base);
unsigned long int strtoul(const char *restrict nptr,
                          char **restrict endptr,
                          int base);
unsigned long long int strtoull(const char *restrict nptr,
                                char **restrict endptr,
                                int base);

int rand(void);
void srand(unsigned int seed);

void *aligned_alloc(size_t alignment,
                    size_t size);
void *calloc(size_t nmemb,
             size_t size);
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr,
              size_t size);

_Noreturn void abort(void);
int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));
_Noreturn void exit(int status);
_Noreturn void _Exit(int status);
_Noreturn void quick_exit(int status);

char *getenv(const char *name);

int system(const char *string);

void *bsearch(const void *key,
              const void *base,
              size_t nmemb,
              size_t size,
              int (*compar)(const void *, const void *));

void qsort(void *base,
           size_t nmemb,
           size_t size,
           int (*compar)(const void *, const void *));

int abs(int j);
long int labs(long int j);
long long int llabs(long long int j);

div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);

int mblen(const char *s, size_t n);
int mbtowc(wchar_t *restrict pwc,
           const char *restrict s,
           size_t n);
int wctomb(char *s, wchar_t wc);
size_t mbstowcs(wchar_t *restrict pwcs,
                const char *restrict s,
                size_t n);
size_t wcstombs(char *restrict s,
                const wchar_t *restrict pwcs,
                size_t n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_STDLIB_H */
