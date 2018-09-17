#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

__attribute__((visibility("hidden")))
int __memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);

int memcmp(const void* __lhs, const void* __rhs, size_t __n) {
  return __memcmp_generic(__lhs, __rhs, __n);
}

}
