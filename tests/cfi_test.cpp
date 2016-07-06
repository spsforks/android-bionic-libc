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
  SYM(void *(*)(), get_last_diag);
  SYM(void *(*)(), get_global_address);
  SYM(void (*)(uint64_t, void *, void *), __cfi_check);
#undef SYM

  int c = get_count();

  // CFI check for code inside the DSO. Can't use just any function address - this is only
  // guaranteed to work for code addresses above __cfi_check.
  void *code_ptr = reinterpret_cast<char *>(__cfi_check) + 1234;
  void *diag_ptr = reinterpret_cast<void *>(5678);
  __cfi_slowpath_diag(42, code_ptr, diag_ptr);
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(code_ptr, get_last_address());
  EXPECT_EQ(diag_ptr, get_last_diag());
  EXPECT_EQ(++c, get_count());

  // __cfi_slowpath passes nullptr for the Diag argument.
  __cfi_slowpath(42, code_ptr);
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(code_ptr, get_last_address());
  EXPECT_EQ(nullptr, get_last_diag());
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

  // Load the same library again.
  void *handle2 = dlopen("libcfi-test.so", RTLD_LAZY | RTLD_LOCAL);
  ASSERT_TRUE(handle2 != nullptr) << dlerror();
  EXPECT_EQ(handle2, handle);

  // Check that it is still there.
  __cfi_slowpath(43, get_global_address());
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(++c, get_count());

  dlclose(handle);
  dlclose(handle2);

  // CFI check for a function inside the unloaded DSO. This is always invalid and gets the process
  // killed.
  EXPECT_DEATH(__cfi_slowpath(45, reinterpret_cast<void *>(code_ptr)), "");
}
