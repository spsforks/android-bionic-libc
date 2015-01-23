#include <math.h>

extern "C" float cosf(float) {
  return NAN;
}

extern "C" double cos(double) {
  return NAN;
}

extern "C" long double cosl(long double) {
  return NAN;
}

