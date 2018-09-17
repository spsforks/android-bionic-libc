#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

/*
static char* hardware = nullptr;
static char buf[4096];

static long __always_inline __open(const char *pathname, int flags) {
    register long r0 __asm__("r0") = reinterpret_cast<long>(pathname);
    register long r1 __asm__("r1") = flags;
    register long r7 __asm__("r7") = __NR_open;
    __asm__("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r7));
    return r0;
}

static ssize_t __always_inline __read(int fd, void *buf, size_t count) {
    register long r0 __asm__("r0") = fd;
    register long r1 __asm__("r1") = reinterpret_cast<long>(buf);
    register long r2 __asm__("r2") = count;
    register long r7 __asm__("r7") = __NR_read;
    __asm__("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r7) : "memory");
    return r0;
}

static char* get_hardware_name() {
    if (hardware != nullptr) return hardware;

    int fd = __open("/proc/cpuinfo", O_RDONLY);
    if (fd == -1) return buf;

    __read(fd, buf, 4096);
    return buf;
}

*/

typedef int (*memcmp_ptr)(const void* __lhs, const void* __rhs, size_t __n);

__attribute__((visibility("hidden")))
int __memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);

int memcmp(const void* __lhs, const void* __rhs, size_t __n)
        __attribute__((ifunc ("memcmp_resolver")));

memcmp_ptr memcmp_resolver() {
    // get_hardware_name();
    // if (buf[0] != 0) {
        return __memcmp_generic;
    // } else {
    //    return nullptr;
    //}
}

}
