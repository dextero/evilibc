#ifndef __EVILIBC_MATH_H
#define __EVILIBC_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 7.12, 2:
 * > The types
 * >     float_t
 * >     double_t
 * > are floating types at least as wide as float and double, respectively,
 * > and such that double_t is at least as wide as float_t. If
 * > FLT_EVAL_METHOD equals 0, float_t and double_t are float and double,
 * > respectively; if FLT_EVAL_METHOD equals 1, they are both double; if
 * > FLT_EVAL_METHOD equals 2, they are both long double; and for other
 * > values of FLT_EVAL_METHOD, they are otherwise implementation-defined. [227]
 * >
 * > [227] The types float_t and double_t are intended to be the
 * > implementation’s most efficient types at least as wide as float and
 * > double, respectively. For FLT_EVAL_METHOD equal 0, 1, or 2, the type
 * > float_t is the narrowest type used by the implementation to evaluate
 * > floating expressions.
 */
#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef double float_t;
typedef long double float_t;
#endif

/*
 * 7.12, 3:
 * > The macro
 * >     HUGE_VAL
 * > expands to a positive double constant expression, not necessarily
 * > representable as a float. The macros
 * >     HUGE_VALF
 * >     HUGE_VALL
 * > are respectively float and long double analogs of HUGE_VAL. [228]
 * >
 * > [228] HUGE_VAL, HUGE_VALF, and HUGE_VALL can be positive infinities in
 * > an implementation that supports infinities.
 *
 * Wait... Unless IEC60559 standard is used, there are actually no restrictions
 * on HUGE_VAL, other than it must be positive. This sounds broken as hell.
 */
#define HUGE_VAL 1.0
#define HUGE_VALF 1.0f
#define HUGE_VALL 1.0L

/*
 * 7.12, 4:
 * > The macro
 * >     INFINITY
 * > expands to a constant expression of type float representing positive or
 * > unsigned infinity, if available; else to a positive constant of type
 * > float that overflows at translation time. [229]
 * >
 * > [229] In this case, using INFINITY will violate the constraint in 6.4.4
 * > and thus require a diagnostic.
 */
#define INFINITY (__builtin_inff())

/*
 * 7.12, 5:
 * > The macro
 * >     NAN
 * > is defined if and only if the implementation supports quiet NaNs for the
 * > float type. It expands to a constant expression of type float
 * > representing a quiet NaN.
 */
#define NAN (__builtin_nanf(""))

/*
 * 7.12, 6:
 * > The number classification macros
 * >     FP_INFINITE
 * >     FP_NAN
 * >     FP_NORMAL
 * >     FP_SUBNORMAL
 * >     FP_ZERO
 * > represent the mutually exclusive kinds of floating-point values. They
 * > expand to integer constant expressions with distinct values. Additional
 * > implementation-defined floating- point classifications, with macro
 * > definitions beginning with FP_ and an uppercase letter, may also be
 * > specified by the implementation.
 */
#define FP_INFINITE  0xf00
#define FP_NAN       0xf01
#define FP_NORMAL    0xf02
#define FP_SUBNORMAL 0xf03
#define FP_ZERO      0xf04

/*
 * 7.12, 7:
 * > The macro
 * >     FP_FAST_FMA
 * > is optionally defined. If defined, it indicates that the fma function
 * > generally executes about as fast as, or faster than, a multiply and an
 * > add of double operands. 230) The macros
 * >     FP_FAST_FMAF
 * >     FP_FAST_FMAL
 * > are, respectively, float and long double analogs of FP_FAST_FMA. If
 * > defined, these macros expand to the integer constant 1.
 */
#if 0
#define FP_FAST_FMA 1
#define FP_FAST_FMAF 1
#define FP_FAST_FMAL 1
#endif

/*
 * 7.12, 8:
 * > The macros
 * >     FP_ILOGB0
 * >     FP_ILOGBNAN
 * > expand to integer constant expressions whose values are returned by
 * > ilogb(x) if x is zero or NaN, respectively. The value of FP_ILOGB0 shall
 * > be either INT_MIN or -INT_MAX. The value of FP_ILOGBNAN shall be either
 * > INT_MAX or INT_MIN.
 */
#define FP_ILOGB0   INT_MIN
#define FP_ILOGBNAN INT_MAX

/*
 * 7.12, 9:
 * > The macros
 * >     MATH_ERRNO
 * >     MATH_ERREXCEPT
 * expand to the integer constants 1 and 2, respectively
 */
#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

/* 7.12, 9:
 * > the macro
 * >     math_errhandling
 * > expands to an expression that has type int and the value MATH_ERRNO,
 * > MATH_ERREXCEPT, or the bitwise OR of both. The value of math_errhandling
 * > is constant for the duration of the program. It is unspecified whether
 * > math_errhandling is a macro or an identifier with external linkage. If a
 * > macro definition is suppressed or a program defines an identifier with
 * > the name math_errhandling, the behavior is undefined. If the expression
 * > math_errhandling & MATH_ERREXCEPT can be nonzero, the implementation
 * > shall define the macros FE_DIVBYZERO, FE_INVALID, and FE_OVERFLOW in
 * > <fenv.h>.
 *
 * TODO: make it an extern int
 */
#define math_errhandling MATH_ERRNO

/* TODO */
#define fpclassify(x) __builtin_fpclassify(x)
#define isfinite(x) __builtin_isfinite(x)
#define isinf(x) __builtin_isinf(x)
#define isnan(x) __builtin_isnan(x)
#define isnormal(x) __builtin_isnormal(x)
#define signbit(x) __builtin_signbit(x)

double acos(double x);
float acosf(float x);
long double acosl(long double x);

double asin(double x);
float asinf(float x);
long double asinl(long double x);

double atan(double x);
float atanf(float x);
long double atanl(long double x);

double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);

double cos(double x);
float cosf(float x);
long double cosl(long double x);

double sin(double x);
float sinf(float x);
long double sinl(long double x);

double tan(double x);
float tanf(float x);
long double tanl(long double x);

double acosh(double x);
float acoshf(float x);
long double acoshl(long double x);

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);

double atanh(double x);
float atanhf(float x);
long double atanhl(long double x);

double cosh(double x);
float coshf(float x);
long double coshl(long double x);

double sinh(double x);
float sinhf(float x);
long double sinhl(long double x);

double tanh(double x);
float tanhf(float x);
long double tanhl(long double x);

double exp(double x);
float expf(float x);
long double expl(long double x);

double exp2(double x);
float exp2f(float x);
long double exp2l(long double x);

double expm1(double x);
float expm1f(float x);
long double expm1l(long double x);

double frexp(double value,
             int *exp);
float frexpf(float value,
             int *exp);
long double frexpl(long double value,
                   int *exp);

int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);

double ldexp(double x,
             int exp);
float ldexpf(float x,
             int exp);
long double ldexpl(long double x,
                   int exp);

double log(double x);
float logf(float x);
long double logl(long double x);

double log10(double x);
float log10f(float x);
long double log10l(long double x);

double log1p(double x);
float log1pf(float x);
long double log1pl(long double x);

double log2(double x);
float log2f(float x);
long double log2l(long double x);

double logb(double x);
float logbf(float x);
long double logbl(long double x);

double modf(double value,
            double *iptr);
float modff(float value,
            float *iptr);
long double modfl(long double value,
                  long double *iptr);

double scalbn(double x,
              int n);
float scalbnf(float x,
              int n);
long double scalbnl(long double x,
                    int n);

double scalbln(double x,
               long int n);
float scalblnf(float x,
               long int n);
long double scalblnl(long double x,
                     long int n);

double cbrt(double x);
float cbrtf(float x);
long double cbrtl(long double x);

double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

double hypot(double x,
             double y);
float hypotf(float x,
             float y);
long double hypotl(long double x,
                   long double y);

double pow(double x,
           double y);
float powf(float x,
           float y);
long double powl(long double x,
                 long double y);

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);

double erf(double x);
float erff(float x);
long double erfl(long double x);

double erfc(double x);
float erfcf(float x);
long double erfcl(long double x);

double lgamma(double x);
float lgammaf(float x);
long double lgammal(long double x);

double tgamma(double x);
float tgammaf(float x);
long double tgammal(long double x);

double ceil(double x);
float ceilf(float x);
long double ceill(long double x);

double floor(double x);
float floorf(float x);
long double floorl(long double x);

double nearbyint(double x);
float nearbyintf(float x);
long double nearbyintl(long double x);

double rint(double x);
float rintf(float x);
long double rintl(long double x);

long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);

long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);

double round(double x);
float roundf(float x);
long double roundl(long double x);

long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);

long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);

double trunc(double x);
float truncf(float x);
long double truncl(long double x);

double fmod(double x,
            double y);
float fmodf(float x,
            float y);
long double fmodl(long double x,
                  long double y);

double remainder(double x,
                 double y);
float remainderf(float x,
                 float y);
long double remainderl(long double x,
                       long double y);

double remquo(double x,
              double y,
              int *quo);
float remquof(float x,
              float y,
              int *quo);
long double remquol(long double x,
                    long double y,
                    int *quo);

double copysign(double x,
                double y);
float copysignf(float x,
                float y);
long double copysignl(long double x,
                      long double y);

double nan(const char *tagp);
float nanf(const char *tagp);
long double nanl(const char *tagp);

double nextafter(double x,
                 double y);
float nextafterf(float x,
                 float y);
long double nextafterl(long double x,
                       long double y);

double nexttoward(double x,
                  long double y);
float nexttowardf(float x,
                  long double y);
long double nexttowardl(long double x,
                        long double y);

double fdim(double x,
            double y);
float fdimf(float x,
            float y);
long double fdiml(long double x,
                  long double y);

double fmax(double x,
            double y);
float fmaxf(float x,
            float y);
long double fmaxl(long double x,
                  long double y);

double fmin(double x,
            double y);
float fminf(float x,
            float y);
long double fminl(long double x,
                  long double y);

double fma(double x,
           double y,
           double z);
float fmaf(float x,
           float y,
           float z);
long double fmal(long double x,
                 long double y,
                 long double z);

/* TODO */
#define isgreater(x, y)
#define isgreaterequal(x, y)
#define isless(x, y)
#define islessequal(x, y)
#define islessgreater(x, y)
#define isunordered(x, y)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EVILIBC_MATH_H */
