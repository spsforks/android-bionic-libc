#include <float.h>
#include <math.h>

/*
 * Some of the "long double" functions are broken when sizeof(long double) == sizeof(double).
 * Android works around those cases by replacing the broken functions with our own trivial stubs
 * that call the regular "double" function.
 */

#if defined(__arm__) || defined(__mips__)

int __fpclassifyl(long double a1) {
  return __fpclassifyd(a1);
}

int __isinfl(long double a1) {
  return isinf(a1);
}

int __isfinitel(long double a1) {
  return __isfinite(a1);
}

int __isnanl(long double a1) {
  return isnan(a1);
}

long double rintl(long double a1) {
  return rint(a1);
}

long double scalbnl(long double a1, int a2) {
  return scalbn(a1, a2);
}

#endif
