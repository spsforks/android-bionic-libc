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
  __cfi_slowpath(42, reinterpret_cast<void *>(get_count));
  EXPECT_EQ(42U, get_last_type_id());
  EXPECT_EQ(reinterpret_cast<void *>(get_count), get_last_address());
  EXPECT_EQ(++c, get_count());

  __cfi_slowpath(43, get_global_address());
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(++c, get_count());

  __cfi_slowpath(44, reinterpret_cast<void *>(&f));
  EXPECT_EQ(43U, get_last_type_id());
  EXPECT_EQ(get_global_address(), get_last_address());
  EXPECT_EQ(c, get_count());

  EXPECT_DEATH(__cfi_slowpath(45, reinterpret_cast<void *>(&c)), "");
}
