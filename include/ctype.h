#ifndef __EVILIBC_CTYPE_H
#define __EVILIBC_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Even though functions below are defined for the EOF constant, it is NOT
 * required for ctype.h to define EOF. See 7.4.
 */

int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int tolower(int c);
int toupper(int c);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_CTYPE_H */
