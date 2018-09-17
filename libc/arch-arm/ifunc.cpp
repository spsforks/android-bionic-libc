#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

static long __always_inline __open(const char *pathname) {
    register long r0 __asm__("r0") = reinterpret_cast<long>(pathname);
    register long r1 __asm__("r1") = O_RDONLY;
    register long r7 __asm__("r7") = __NR_open;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r7));
    return r0;
}

static ssize_t __always_inline __read(int fd, void *buf, size_t count) {
    register long r0 __asm__("r0") = fd;
    register long r1 __asm__("r1") = reinterpret_cast<long>(buf);
    register long r2 __asm__("r2") = count;
    register long r7 __asm__("r7") = __NR_read;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r7) : "memory");
    return r0;
}

static int __always_inline __close(int fd) {
    register long r0 __asm__("r0") = fd;
    register long r7 __asm__("r7") = __NR_close;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r7));
    return r0;
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
