#ifndef __EVILIBC_UNDEFINED_BEHAVIOR_H
#define __EVILIBC_UNDEFINED_BEHAVIOR_H

/* Undefined Behavior handler. */
void __evil_ub(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

/* Implementation-defined Behavior handler. */
void __evil_idb(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

#endif /* __EVILIBC_UNDEFINED_BEHAVIOR_H */
