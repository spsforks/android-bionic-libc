#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// This library is built for all targets, including host tests, so __cfi_slowpath may not be
// present. But it is only used in the bionic loader tests.
extern "C" __attribute__((weak)) void __cfi_slowpath(uint64_t, void*);

static int g_count;

// Mock a CFI-enabled library without relying on the compiler.
extern "C" __attribute__((aligned(4096))) void __cfi_check(uint64_t /*CallSiteTypeId*/,
                                                           void* /*TargetAddr*/, void* /*Diag*/) {
  ++g_count;
}

void preinit_ctor() {
  assert(g_count == 0);
  __cfi_slowpath(42, reinterpret_cast<void*>(&preinit_ctor));
  assert(g_count == 1);
}

__attribute__((section(".preinit_array"), used)) void (*preinit_ctor_p)(void) = preinit_ctor;

__attribute__((constructor, used)) void ctor() {
  assert(g_count == 1);
  __cfi_slowpath(42, reinterpret_cast<void*>(&ctor));
  assert(g_count == 2);
}

int main(void) {
  assert(g_count == 2);
  __cfi_slowpath(42, reinterpret_cast<void*>(&main));
  assert(g_count == 3);
  return 0;
}
