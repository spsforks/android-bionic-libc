#include <stddef.h>

extern "C" {

typedef int (*memcmp_ptr)(const void* __lhs, const void* __rhs, size_t __n);
int memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);
int memcmp_sse4(const void* __lhs, const void* __rhs, size_t __n);

int memcmp(const void* __lhs, const void* __rhs, size_t __n)
    __attribute__((ifunc ("memcmp_resolver")));

memcmp_ptr memcmp_resolver() {
  __builtin_cpu_init();
  if (__builtin_cpu_supports("sse4.1")) {
    return memcmp_sse4;
  }
  return memcmp_generic;
}

}
