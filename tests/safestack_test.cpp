#include <gtest/gtest.h>
#include "BionicDeathTest.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__BIONIC__)

extern "C" void** __safestack_pointer_address();

bool safestack_enabled;

void safestack_test_preinit() {
  safestack_enabled = *__safestack_pointer_address();
}

__attribute__((section(".preinit_array"), used))
void (*safestack_test_preinit_p)(void) = safestack_test_preinit;

void test_unsafe_stack(void* p) {
  // Return if safestack is disabled.
  if (p == nullptr) return;
  EXPECT_NE(nullptr, p);
  // Points to a mapped page.
  *(reinterpret_cast<char*>(p) - 100) = 42;
}

static void* ThreadFn(void*) {
  test_unsafe_stack(*__safestack_pointer_address());
  return nullptr;
}
#endif // BIONIC

// Test that if SafeStack is enabled, the pointer was non-zero in
// .preinit_array, too. And vice versa.
TEST(safestack, enabled_or_not) {
#if defined(__BIONIC__)
  bool still_enabled = *__safestack_pointer_address();
  ASSERT_EQ(safestack_enabled, still_enabled);
#endif // BIONIC
}

TEST(safestack, simple) {
#if defined(__BIONIC__)
  test_unsafe_stack(*__safestack_pointer_address());
#endif // BIONIC
}

TEST(safestack, threads) {
#if defined(__BIONIC__)
  pthread_t t;
  pthread_create(&t, nullptr, ThreadFn, nullptr);
  pthread_join(t, nullptr);
#endif // BIONIC
}
