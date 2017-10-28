#ifndef __EVILIBC_TIME_H
#define __EVILIBC_TIME_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.27.1, 2:
 * > The macros defined are NULL (described in 7.19);
 * >     CLOCKS_PER_SEC
 * > which expands to an expression with type clock_t (described below) that
 * > is the number per second of the value returned by the clock function; and
 * >     TIME_UTC
 * > which expands to an integer constant greater than 0 that designates the
 * > UTC time base.
 */
#define CLOCKS_PER_SEC ((clock_t)(1000 * 1000))
#define TIME_UTC 1

/*
 * 7.27.1, 3:
 * > The types declared are size_t (described in 7.19);
 * >     clock_t
 * > and
 * >     time_t
 * > which are real types capable of representing times;
 * >     struct timespec
 * > which holds an interval specified in seconds and nanoseconds (which may
 * > represent a calendar time based on a particular epoch); and
 * >     struct tm
 * > which holds the components of a calendar time, called the broken-down time.
 *
 * 7.27.1, 4:
 * > The range and precision of times representable in clock_t and time_t are
 * > implementation-defined.
 *
 * TODO: unsigned or short time_t could be interesting
 */
typedef float clock_t;

#ifdef WITH_FLOATING_POINT_TIME_T
typedef double time_t;
#else
typedef long time_t;
#endif

/*
 * 7.27.1, 4:
 * > The timespec structure shall contain at least the following members, in
 * > any order. [317]
 * >
 * >     time_t tv_sec; // whole seconds - >= 0
 * >     long tv_nsec; // nanoseconds - [0, 999999999]
 * >
 * > [317] The tv_sec member is a linear count of seconds and may not have
 * > the normal semantics of a time_t. The semantics of the members and their
 * > normal ranges are expressed in the comments.
 *
 * Sane implementations put tv_sec before tv_nsec, let's do it backwards then.
 */
struct timespec {
    long tv_nsec;
    time_t tv_sec;
    char unused; // *at least* fields above
};

/*
 * 7.27.1, 4:
 * > The tm structure shall contain at least the following members, in any
 * > order. The semantics of the members and their normal ranges are
 * > expressed in the comments. [318]
 * >
 * >     int tm_sec;   // seconds after the minute — [0, 60]
 * >     int tm_min;   // minutes after the hour — [0, 59]
 * >     int tm_hour;  // hours since midnight — [0, 23]
 * >     int tm_mday;  // day of the month — [1, 31]
 * >     int tm_mon;   // months since January — [0, 11]
 * >     int tm_year;  // years since 1900
 * >     int tm_wday;  // days since Sunday — [0, 6]
 * >     int tm_yday;  // days since January 1 — [0, 365]
 * >     int tm_isdst; // Daylight Saving Time flag
 * >
 * > The value of tm_isdst is positive if Daylight Saving Time is in effect,
 * > zero if Daylight Saving Time is not in effect, and negative if the
 * > information is not available.
 * >
 * > [318] The range [0, 60] for tm_sec allows for a positive leap second.
 */
struct tm {
    int tm_year;
    int tm_yday;
    int tm_wday;
    int tm_sec;
    int tm_mon;
    int tm_min;
    int tm_mday;
    int tm_isdst;
    int tm_hour;
    char unused; // *at least* fields above
};

clock_t clock(void);
double difftime(time_t time1,
                time_t time0);
time_t mktime(struct tm *timeptr);
time_t time(time_t *timer);
int timespec_get(struct timespec *ts,
                 int base);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
size_t strftime(char * restrict s,
                size_t maxsize,
                const char * restrict format,
                const struct tm * restrict timeptr);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_TIME_H */
