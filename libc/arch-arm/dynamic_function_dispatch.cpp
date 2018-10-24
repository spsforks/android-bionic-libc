/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <fcntl.h>
#include <sys/syscall.h>

extern "C" {

enum CPU_VARIANT {
    UNKNOWN = 0,
    GENERIC,
    CORTEX_A7,
    CORTEX_A9,
    CORTEX_A53,
    CORTEX_A55,
    DENVER,
    KRAIT,
    KRYO,
};

static constexpr int MAX_CPU_NAME_LEN = 12;
struct CPU_VARIANT_NAMES {
    char name[MAX_CPU_NAME_LEN];
    enum CPU_VARIANT variant;
};

static constexpr struct CPU_VARIANT_NAMES cpu_variant_names[] = {
    {"cortex-a76", CORTEX_A55},
    {"cortex-a75", CORTEX_A55},
    {"kryo", KRYO},
    {"cortex-a73", CORTEX_A55},
    {"cortex-a55", CORTEX_A55},
    {"cortex-a53", CORTEX_A53},
    {"krait", KRAIT},
    {"cortex-a9", CORTEX_A9},
    {"cortex-a7", CORTEX_A7},
    {"denver", DENVER},
    // UNKNOWN indicates the end of this array.
    {"", UNKNOWN},
};

static long ifunc_open(const char* pathname) {
    register long r0 __asm__("r0") = AT_FDCWD;
    register long r1 __asm__("r1") = reinterpret_cast<long>(pathname);
    register long r2 __asm__("r2") = O_RDONLY;
    register long r3 __asm__("r3") = 0;
    register long r7 __asm__("r7") = __NR_openat;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r7));
    return r0;
}

static ssize_t ifunc_read(int fd, void* buf, size_t count) {
    register long r0 __asm__("r0") = fd;
    register long r1 __asm__("r1") = reinterpret_cast<long>(buf);
    register long r2 __asm__("r2") = count;
    register long r7 __asm__("r7") = __NR_read;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r7) : "memory");
    return r0;
}

static int ifunc_close(int fd) {
    register long r0 __asm__("r0") = fd;
    register long r7 __asm__("r7") = __NR_close;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r7));
    return r0;
}

#define DEFINE_IFUNC(name) \
    name##_func name __attribute__((ifunc(#name "_resolver"))); \
    __attribute__((visibility("hidden"))) \
    name##_func* name##_resolver()

#define DECLARE_FUNC(type, name) \
    __attribute__((visibility("hidden"))) \
    type name

#define RETURN_FUNC(type, name) { \
        DECLARE_FUNC(type, name); \
        return name; \
    }

static enum CPU_VARIANT init_cpu_variant() {
    int fd = ifunc_open("/dev/cpu_variant:arm");
    if (fd < 0) return GENERIC;

    char name[MAX_CPU_NAME_LEN];

    int bytes_read, total_read = 0;
    while (total_read < MAX_CPU_NAME_LEN - 1 &&
           (bytes_read = ifunc_read(fd, name + total_read,
                                    MAX_CPU_NAME_LEN - 1 - total_read)) > 0) {
        total_read += bytes_read;
    }
    ifunc_close(fd);

    if (bytes_read != 0) {
        // The file is too big. We haven't reach the end. Or maybe there is an
        // error when reading.
        return GENERIC;
    }
    name[total_read] = 0;

    typedef int strcmp_func(const char* __lhs, const char* __rhs);
    DECLARE_FUNC(strcmp_func, strcmp_a15);

    const struct CPU_VARIANT_NAMES* cpu_variant = cpu_variant_names;
    while (cpu_variant->variant != UNKNOWN) {
        if (strcmp_a15(cpu_variant->name, name) == 0) {
            return cpu_variant->variant;
        }
        cpu_variant++;
    }
    return GENERIC;
}

static enum CPU_VARIANT get_cpu_variant() {
    static enum CPU_VARIANT cpu_variant = UNKNOWN;
    if (cpu_variant == UNKNOWN) {
        cpu_variant = init_cpu_variant();
    }
    return cpu_variant;
}

typedef void* memmove_func(void* __dst, const void* __src, size_t __n);
DEFINE_IFUNC(memmove) {
    switch(get_cpu_variant()) {
        case CORTEX_A7:
            RETURN_FUNC(memmove_func, memmove_a7);
        case CORTEX_A9:
            RETURN_FUNC(memmove_func, memmove_a9);
        case KRAIT:
            RETURN_FUNC(memmove_func, memmove_krait);
        case CORTEX_A53:
            RETURN_FUNC(memmove_func, memmove_a53);
        case CORTEX_A55:
        case DENVER:
            RETURN_FUNC(memmove_func, memmove_denver);
        case KRYO:
            RETURN_FUNC(memmove_func, memmove_kryo);
        default:
            RETURN_FUNC(memmove_func, memmove_a15);
    }
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC(memcpy) {
    return memmove_resolver();
}

typedef void* __memset_chk_func(void* s, int c, size_t n, size_t n2);
DEFINE_IFUNC(__memset_chk) {
    switch(get_cpu_variant()) {
        case CORTEX_A7:
        case CORTEX_A53:
        case CORTEX_A55:
        case KRYO:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a7);
        case CORTEX_A9:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a9);
        case KRAIT:
            RETURN_FUNC(__memset_chk_func, __memset_chk_krait);
        case DENVER:
            RETURN_FUNC(__memset_chk_func, __memset_chk_denver);
        default:
            RETURN_FUNC(__memset_chk_func, __memset_chk_a15);
    }
}

typedef void* memset_func(void* __dst, int __ch, size_t __n);
DEFINE_IFUNC(memset) {
    switch(get_cpu_variant()) {
        case CORTEX_A7:
        case CORTEX_A53:
        case CORTEX_A55:
        case KRYO:
             RETURN_FUNC(memset_func, memset_a7);
        case CORTEX_A9:
             RETURN_FUNC(memset_func, memset_a9);
        case KRAIT:
             RETURN_FUNC(memset_func, memset_krait);
        case DENVER:
             RETURN_FUNC(memset_func, memset_denver);
        default:
             RETURN_FUNC(memset_func, memset_a15);
    }
}

typedef char* strcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC(strcpy) {
    switch(get_cpu_variant()) {
        case CORTEX_A9:
            RETURN_FUNC(strcpy_func, strcpy_a9);
        default:
            RETURN_FUNC(strcpy_func, strcpy_a15);
    }
}

typedef char* stpcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC(stpcpy) {
    switch(get_cpu_variant()) {
        case CORTEX_A9:
            RETURN_FUNC(stpcpy_func, stpcpy_a9);
        default:
            RETURN_FUNC(stpcpy_func, stpcpy_a15);
    }
}

typedef char* strcat_func(char* __dst, const char* __src);
DEFINE_IFUNC(strcat) {
    switch(get_cpu_variant()) {
        case CORTEX_A9:
            RETURN_FUNC(strcat_func, strcat_a9);
        default:
            RETURN_FUNC(strcat_func, strcat_a15);
    }
}

typedef int strcmp_func(const char* __lhs, const char* __rhs);
DEFINE_IFUNC(strcmp) {
    switch(get_cpu_variant()) {
        case CORTEX_A9:
            RETURN_FUNC(strcmp_func, strcmp_a9);
        case CORTEX_A55:
        case KRAIT:
        case KRYO:
            RETURN_FUNC(strcmp_func, strcmp_krait);
        default:
            RETURN_FUNC(strcmp_func, strcmp_a15);
    }
}

typedef size_t strlen_func(const char* __s);
DEFINE_IFUNC(strlen) {
    switch(get_cpu_variant()) {
        case CORTEX_A9:
            RETURN_FUNC(strlen_func, strlen_a9);
        default:
            RETURN_FUNC(strlen_func, strlen_a15);
    }
}

}  // extern "C"
