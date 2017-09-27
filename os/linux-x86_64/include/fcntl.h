#ifndef _FCNTL_H
#define _FCNTL_H
#include <bits/fcntl.h>

/* TODO: copy-paste from Linux fcntl.h */
# ifndef R_OK			/* Verbatim from <unistd.h>.  Ugh.  */
/* Values for the second argument to access.
   These may be OR'd together.  */
#  define R_OK	4		/* Test for read permission.  */
#  define W_OK	2		/* Test for write permission.  */
#  define X_OK	1		/* Test for execute permission.  */
#  define F_OK	0		/* Test for existence.  */
# endif
#endif // _FCNTL_H
