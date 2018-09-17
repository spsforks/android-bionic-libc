#include <string.h>

__attribute__ ((noinline))
int test1(const char* a, const char* b, int sz) {
  return __builtin_memcmp(a, b, sz);
}

__attribute__ ((noinline))
int test2(const char* a, const char* b, int sz) {
  return memcmp(a, b, sz);
}

int main() {
  return test1("abc", "abc", 4) + test2("abc", "abc", 4);
}
