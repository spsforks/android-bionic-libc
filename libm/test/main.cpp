#include <stdio.h>
#include <math.h>

int main() {
  int exit_code = 0;

  // Our test overrides always returns a nan value.
  double value = acos(1.0);
  if (!isnan(value)) {
    printf("acos override failed, did not get nan value from cos\n");
    exit_code++;
  } else {
    printf("acos override pass\n");
  }

  long double valuel = acosf(1.0);
  if (!isnan(valuel)) {
    printf("acosf override failed, did not get nan value from cosl\n");
    exit_code++;
  } else {
    printf("acosf override pass\n");
  }

  return exit_code;
}
