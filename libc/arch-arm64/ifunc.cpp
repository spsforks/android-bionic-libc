#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

static long __always_inline __open(const char *pathname) {
    register long x0 __asm__("x0") = AT_FDCWD;
    register long x1 __asm__("x1") = reinterpret_cast<long>(pathname);
    register long x2 __asm__("x2") = O_RDONLY;
    register long x3 __asm__("x3") = 0;
    register long x8 __asm__("x8") = __NR_openat;
    __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x8));
    return x0;
}

static ssize_t __always_inline __read(int fd, void *buf, size_t count) {
    register long x0 __asm__("x0") = fd;
    register long x1 __asm__("x1") = reinterpret_cast<long>(buf);
    register long x2 __asm__("x2") = count;
    register long x8 __asm__("x8") = __NR_read;
    __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
    return x0;
}

static int __always_inline __close(int fd) {
    register long x0 __asm__("x0") = fd;
    register long x8 __asm__("x8") = __NR_close;
    __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x8));
    return x0;
}

enum MACHINE_TYPE {
    UNKNOWN = 0,
    GENERIC = 1,
};

static char buf[8];
static MACHINE_TYPE get_hardware_type() {
    int fd = __open("/proc/cpuinfo");
    if (fd == -1) return UNKNOWN;

    __read(fd, buf, 8);
    __close(fd);

    if (buf[0] != 0) return GENERIC;
    return UNKNOWN;
}

typedef int (*memcmp_ptr)(const void* __lhs, const void* __rhs, size_t __n);

__attribute__((visibility("hidden")))
int __memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);

int memcmp(const void* __lhs, const void* __rhs, size_t __n)
        __attribute__((ifunc ("memcmp_resolver")));

memcmp_ptr memcmp_resolver() {
    MACHINE_TYPE type = get_hardware_type();
    switch (type) {
        case GENERIC:
            return __memcmp_generic;
        default:
            // Should log and return generic.
            return nullptr;
    }
}

}
