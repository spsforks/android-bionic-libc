#include <float.h>
#include <math.h>

#if (LDBL_MANT_DIG == 53)

/* long double == double, so fake it. */

long double rintl(long double a1) {
  return rint(a1);
}

long double scalbnl(long double a1, int a2) {
  return scalbn(a1, a2);
}

#endif
