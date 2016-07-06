#include <stdint.h>

static int g_count;
static uint64_t g_last_type_id;
static void *g_last_address;

extern "C" {

// Mock a CFI-enabled library without relying on the compiler.
__attribute__((aligned(4096)))
void __cfi_check(uint64_t CallSiteTypeId, void *TargetAddr) {
  ++g_count;
  g_last_type_id = CallSiteTypeId;
  g_last_address = TargetAddr;
}

int get_count() {
  return g_count;
}

// FIXME: check ctor/dtor

uint64_t get_last_type_id() {
  return g_last_type_id;
}

void *get_last_address() {
  return g_last_address;
}

void *get_global_address() {
  return &g_count;
}

}
