#include <gtest/gtest.h>
#include <dlfcn.h>

#include "BionicDeathTest.h"

static void f() {}

TEST(cfi_test, basic) {
  void *handle;
  handle = dlopen("libcfi-test.so", RTLD_LAZY | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();

#define SYM(type, name) auto name = reinterpret_cast<type>(dlsym(handle, #name))
  SYM(int (*)(), get_count);
  SYM(uint64_t (*)(), get_last_type_id);
  SYM(void *(*)(), get_last_address);
  SYM(void *(*)(), get_global_address);
#undef SYM

  int c = get_count();

  // CFI check for a function inside the DSO.
  __cfi_slowpath(42, reinterpret_cast<void *>(get_count));
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(reinterpret_cast<void *>(get_count), get_last_address());
  EXPECT_EQ(++c, get_count());

  // CFI check for a data address inside the DSO.
  __cfi_slowpath(43, get_global_address());
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(++c, get_count());

  // CFI check for a function inside _this_ DSO. It either goes to this DSO's __cfi_check,
  // or (if missing) is simply ignored. Any way, it does not affect the test lib's counters.
  __cfi_slowpath(44, reinterpret_cast<void *>(&f));
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(c, get_count());

  // CFI check for a stack address. This is always invalid and gets the process killed.
  EXPECT_DEATH(__cfi_slowpath(45, reinterpret_cast<void *>(&c)), "");

  // CFI check for a heap address. This is always invalid and gets the process killed.
  void *p = malloc(4096);
  EXPECT_DEATH(__cfi_slowpath(46, p), "");
  free(p);

  dlclose(handle);

  // CFI check for a function inside the unloaded DSO. This is always invalid and gets the process
  // killed.
  EXPECT_DEATH(__cfi_slowpath(45, reinterpret_cast<void *>(get_count)), "");
}
