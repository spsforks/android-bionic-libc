#include <math.h>

extern "C" double acos(double) {
  return NAN;
}

extern "C" float acosf(float) {
  return NAN;
}
