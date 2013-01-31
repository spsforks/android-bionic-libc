#include <float.h>
#include <math.h>

extern int __isinf(double); /* isinf.c */

int (isinf)(double a1) {
  return __isinf(a1);
}

/*
 * Some of the "long double" functions are broken when sizeof(long double) == sizeof(double).
 * Android works around those cases by replacing the broken functions with our own trivial stubs
 * that call the regular "double" function.
 */

#if defined(__arm__) || defined(__mips__) || defined(__i386__)

int __fpclassifyl(long double a1) {
  return __fpclassifyd(a1);
}

int __isfinitel(long double a1) {
  return __isfinite(a1);
}

int __isinfl(long double a1) {
  return __isinf(a1);
}

int __isnanl(long double a1) {
  return isnan(a1);
}

int __isnormall(long double a1) {
  return __isnormal(a1);
}

long double fmaxl(long double a1, long double a2) {
  return fmax(a1, a2);
}

long double fminl(long double a1, long double a2) {
  return fmin(a1, a2);
}

long double rintl(long double a1) {
  return rint(a1);
}

long double scalbnl(long double a1, int a2) {
  return scalbn(a1, a2);
}

long double sqrtl(long double a1) {
  return sqrt(a1);
}

#endif
