#include <fcntl.h>

extern "C" {

int __openat(int, const char*, int, int);
ssize_t read(int, void*, size_t);

static char* hardware = nullptr;
static char buf[4096];

static char* get_hardware_name() {
  if (hardware != nullptr) return hardware;

  int fd = __openat(AT_FDCWD, "/proc/cpuinfo", O_RDONLY, 0);
  if (fd == -1) return buf;

  read(fd, buf, 4096);
  return buf;
}

typedef int (*memcmp_ptr)(const void* __lhs, const void* __rhs, size_t __n);
int memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);

int memcmp(const void* __lhs, const void* __rhs, size_t __n)
        __attribute__((ifunc ("memcmp_resolver")));

memcmp_ptr memcmp_resolver() {
    get_hardware_name();
    if (buf[0] != 0) {
      // Returns the right function if we read anything.
      return memcmp_generic;
    }
    // This probably will lead to a crash somewhere.
    return nullptr;
}

}
