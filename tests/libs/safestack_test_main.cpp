#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#if SAFESTACK_EXEC == 2 || SAFESTACK_LIB == 2
#define SAFESTACK_REQUIRED 1
#endif

#if SAFESTACK_EXEC != 0 && SAFESTACK_LIB != 0
#define SAFESTACK_ALL 1
#endif

enum { NO = 0, YES = 1, REQ = 2 };

void try_load(int level, bool expect_success) {
  const char* lib;
  if (level == NO)
    lib = "libtest_safestack_dl_no.so";
  else if (level == YES)
    lib = "libtest_safestack_dl_yes.so";
  else if (level == REQ)
    lib = "libtest_safestack_dl_req.so";
  void* handle = dlopen(lib, RTLD_NOW);
  if ((handle != nullptr) != expect_success) {
    exit(1);
  }
}

int main() {
#ifdef SAFESTACK_REQUIRED
  // SafeStack required from the start.
  try_load(NO, false);
  try_load(YES, true);
  try_load(REQ, true);
#else
#ifdef SAFESTACK_ALL
  // SafeStack not required from the start, but all initial libraries are built with it.
  try_load(REQ, true);
  try_load(NO, false);
  try_load(YES, true);
#else
  // SafeStack not required from the start, and there are non-SafeStack libraries.
  try_load(REQ, false);
  try_load(NO, true);
  try_load(YES, true);
#endif
#endif
  printf("safestack test done\n");
}
