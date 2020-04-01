#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "gdtoa.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Inputs: d, xdigs, ndigits
  // Outputs: decpt, sign, rve

  //__hdtoa(double d, const char* xdigs, int ndigits, int* decpt, int* sign, char** rve);

  // if (size < sizeof(double) + sizeof(int)) {
  if (size < sizeof(double) + sizeof(int) + 1) {
    return 0;
  }

  size_t i = 0;

  double d;
  memcpy((void*)&d, (void*)&data[i], sizeof(double));
  i += sizeof(double);

  int ndigits;
  memcpy((void*)&ndigits, (void*)&data[i], sizeof(int));
  i += sizeof(int);

  int mode = data[i] % 10;

  int decpt;
  int sign;
  char* rve;

  //__hdtoa(d, "0123456789ABCDEF", ndigits, &decpt, &sign, &rve);
  __dtoa(d, mode, ndigits, &decpt, &sign, &rve);

  return 0;
}
