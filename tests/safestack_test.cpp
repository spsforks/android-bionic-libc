#include <gtest/gtest.h>
#include "BionicDeathTest.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__BIONIC__)

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

static bool safestack_enabled() {
  char v[PROP_VALUE_MAX];
  static bool enabled =
    __system_property_get("ro.libc.safestack", v) && atoi(v) != 0;
  return enabled;
}

extern "C" void **__safestack_pointer_address();

void test_unsafe_stack(void *p) {
  if (safestack_enabled()) {
    EXPECT_NE(nullptr, p);
    // Points to a mapped page.
    *((char*)p - 100) = 42;
  } else {
    EXPECT_EQ(nullptr, p);
  }
}

TEST(safestack, simple) {
  test_unsafe_stack(*__safestack_pointer_address());
}

static void *ThreadFn(void *) {
  test_unsafe_stack(*__safestack_pointer_address());
  return nullptr;
}

TEST(safestack, threads) {
  if (!safestack_enabled()) return;
  pthread_t t;
  pthread_create(&t, nullptr, ThreadFn, nullptr);
  pthread_join(t, nullptr);
}
#endif
