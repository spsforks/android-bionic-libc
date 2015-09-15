#include <gtest/gtest.h>
#include "BionicDeathTest.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__BIONIC__) && defined(BIONIC_SAFESTACK)

extern "C" void** __safestack_pointer_address();

void test_unsafe_stack(void* p) {
  EXPECT_NE(nullptr, p);
  // Points to a mapped page.
  *(reinterpret_cast<char*>(p) - 100) = 42;
}

TEST(safestack, simple) {
  test_unsafe_stack(*__safestack_pointer_address());
}

static void* ThreadFn(void*) {
  test_unsafe_stack(*__safestack_pointer_address());
  return nullptr;
}

TEST(safestack, threads) {
  pthread_t t;
  pthread_create(&t, nullptr, ThreadFn, nullptr);
  pthread_join(t, nullptr);
}
#endif
