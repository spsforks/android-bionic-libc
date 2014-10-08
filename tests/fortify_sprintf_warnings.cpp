#define _FORTIFY_SOURCE 2
#include <stdio.h>

void test_sprintf(void) {
  char buf[4];

  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  sprintf(buf, "foobar");

  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  sprintf(buf, "%s", "foobar");
}

void test_snprintf(void) {
  char buf[4];

  // GCC: warning: call to int __builtin___snprintf_chk(char*, unsigned int, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "foobar");

  // GCC: warning: call to int __builtin___snprintf_chk(char*, unsigned int, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%s", "foobar");

  // GCC: warning: call to int __builtin___snprintf_chk(char*, unsigned int, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, " %s ", "foobar");

  // GCC: warning: call to int __builtin___snprintf_chk(char*, unsigned int, int, unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%d", 100000);
}
