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

const int MAX_CPU_NAME_LEN = 16;
struct CPU_VARIANT_NAMES {
    char name[MAX_CPU_NAME_LEN];
    enum CPU_VARIANT variant;
};

static struct CPU_VARIANT_NAMES cpu_variant_names[] = {
    {"kryo", KRYO},
    {"denver", DENVER},
    {"cortex-a76", CORTEX_A55},
    {"cortex-a75", CORTEX_A55},
    {"cortex-a73", CORTEX_A55},
    {"cortex-a55", CORTEX_A55},
    {"cortex-a53", CORTEX_A53},
    {"krait", KRAIT},
    {"cortex-a9", CORTEX_A9},
    {"cortex-a7", CORTEX_A7},
    // UNKNOWN indicates the end of this array.
    {"", UNKNOWN},
};

static long __always_inline __open(const char *pathname) {
    register long r0 __asm__("r0") = AT_FDCWD;
    register long r1 __asm__("r1") = (long)pathname;
    register long r2 __asm__("r2") = O_RDONLY;
    register long r3 __asm__("r3") = 0;
    register long r7 __asm__("r7") = __NR_openat;
    __asm__ volatile("swi #0" : "=r"(r0) : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r7));
    return r0;
}

static ssize_t __always_inline __read(int fd, void *buf, size_t count) {
    register long r0 __asm__("r0") = fd;
    register long r1 __asm__("r1") = (long)(buf);
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

#define DEFINE_IFUNC(name) \
    name##_func name __attribute__((ifunc(#name "_resolver"))); \
    __attribute__((visibility("hidden"))) \
    name##_func* name##_resolver()

#define START_IFUNC_SWITCH(name) \
    DEFINE_IFUNC(name) { \
        switch(get_cpu_variant()) {

#define END_IFUNC_SWITCH(name) } }

#define DECLARE_FUNC(type, name) \
        __attribute__((visibility("hidden"))) \
        type name

#define RETURN_FUNC(type, name) { \
        DECLARE_FUNC(type, name); \
        return name; \
    }

static enum CPU_VARIANT init_cpu_variant() {
    int fd = __open("/dev/cpu_variant.arm");
    if (fd == -1) return GENERIC;

    char name[MAX_CPU_NAME_LEN];

    int bytes_read, total_read = 0;
    while (total_read < MAX_CPU_NAME_LEN - 1 &&
           (bytes_read = __read(fd, name + total_read,
                                MAX_CPU_NAME_LEN - 1 - total_read)) > 0) {
        total_read += bytes_read;
    }
    __close(fd);

    if (bytes_read != 0) {
        // The file is too big. We haven't reach the end. Or maybe there is an
        // error when reading.
        return GENERIC;
    }
    name[total_read] = 0;

    typedef int strcmp_func(const char* __lhs, const char* __rhs);
    DECLARE_FUNC(strcmp_func, strcmp_a15);

    struct CPU_VARIANT_NAMES *cpu_variant = cpu_variant_names;
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
START_IFUNC_SWITCH(memmove)
    // arch-arm/cortex-a7/bionic/memmove.S
    case CORTEX_A7: RETURN_FUNC(memmove_func, memmove_a7);
    // arch-arm/cortex-a9/bionic/memmove.S
    case CORTEX_A9: RETURN_FUNC(memmove_func, memmove_a9);
    // arch-arm/krait/bionic/memmove.S
    case KRAIT: RETURN_FUNC(memmove_func, memmove_krait);
    // arch-arm/cortex-a53/bionic/memmove.S
    case CORTEX_A53: RETURN_FUNC(memmove_func, memmove_a53);

    case CORTEX_A55:
    case DENVER:
	// arch-arm/denver/bionic/memmove.S
        RETURN_FUNC(memmove_func, memmove_denver);
    // arch-arm/kryo/bionic/memmove.S
    case KRYO: RETURN_FUNC(memmove_func, memmove_kryo);
    // arch-arm/cortex-a15/bionic/memmove.S
    default: RETURN_FUNC(memmove_func, memmove_a15);
END_IFUNC_SWITCH(memmove)

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC(memcpy) {
    return memmove_resolver();
}

typedef void* __memset_chk_func(void *s, int c, size_t n, size_t n2);
START_IFUNC_SWITCH(__memset_chk)
    case CORTEX_A7:
    case CORTEX_A53:
    case CORTEX_A55:
    case KRYO:
         // arch-arm/cortex-a7/bionic/memset.S
         RETURN_FUNC(__memset_chk_func, __memset_chk_a7);
    // arch-arm/cortex-a9/bionic/memset.S
    case CORTEX_A9: RETURN_FUNC(__memset_chk_func, __memset_chk_a9);
    // arch-arm/krait/bionic/memset.S
    case KRAIT: RETURN_FUNC(__memset_chk_func, __memset_chk_krait);
    // arch-arm/denver/bionic/memset.S
    case DENVER: RETURN_FUNC(__memset_chk_func, __memset_chk_denver);
    // arch-arm/cortex-a15/bionic/memset.S
    default: RETURN_FUNC(__memset_chk_func, __memset_chk_a15);
END_IFUNC_SWITCH(__memset_chk)

typedef void* memset_func(void* __dst, int __ch, size_t __n);
START_IFUNC_SWITCH(memset)
    case CORTEX_A7:
    case CORTEX_A53:
    case CORTEX_A55:
    case KRYO:
         // arch-arm/cortex-a7/bionic/memset.S
         RETURN_FUNC(memset_func, memset_a7);
    // arch-arm/cortex-a9/bionic/memset.S
    case CORTEX_A9: RETURN_FUNC(memset_func, memset_a9);
    // arch-arm/krait/bionic/memset.S
    case KRAIT: RETURN_FUNC(memset_func, memset_krait);
    // arch-arm/denver/bionic/memset.S
    case DENVER: RETURN_FUNC(memset_func, memset_denver);
    // arch-arm/cortex-a15/bionic/memset.S
    default: RETURN_FUNC(memset_func, memset_a15);
END_IFUNC_SWITCH(memset)

typedef char* strcpy_func(char* __dst, const char* __src);
START_IFUNC_SWITCH(strcpy)
    // arch-arm/cortex-a9/bionic/strcpy.S
    case CORTEX_A9: RETURN_FUNC(strcpy_func, strcpy_a9);
    // arch-arm/cortex-a15/bionic/string_copy.S
    default: RETURN_FUNC(strcpy_func, strcpy_a15);
END_IFUNC_SWITCH(strcpy)

typedef char* stpcpy_func(char* __dst, const char* __src);
START_IFUNC_SWITCH(stpcpy)
    // arch-arm/cortex-a9/bionic/string_copy.S
    case CORTEX_A9: RETURN_FUNC(stpcpy_func, stpcpy_a9);
    // arch-arm/cortex-a15/bionic/stpcpy.S
    default: RETURN_FUNC(stpcpy_func, stpcpy_a15);
END_IFUNC_SWITCH(stpcpy)

typedef char* strcat_func(char* __dst, const char* __src);
START_IFUNC_SWITCH(strcat)
    // arch-arm/cortex-a9/bionic/strcat.S
    case CORTEX_A9: RETURN_FUNC(strcat_func, strcat_a9);
    // arch-arm/cortex-a15/bionic/strcat.S
    default: RETURN_FUNC(strcat_func, strcat_a15);
END_IFUNC_SWITCH(strcat)

typedef int strcmp_func(const char* __lhs, const char* __rhs);
START_IFUNC_SWITCH(strcmp)
    // arch-arm/cortex-a9/bionic/strcmp.S
    case CORTEX_A9: RETURN_FUNC(strcmp_func, strcmp_a9);

    case CORTEX_A55:
    case KRAIT:
    case KRYO:
        // arch-arm/krait/bionic/strcmp.S
        RETURN_FUNC(strcmp_func, strcmp_krait);
    // This implementation is also used in init_cpu_variant().
    // arch-arm/cortex-a15/bionic/strcmp.S
    default: RETURN_FUNC(strcmp_func, strcmp_a15);
END_IFUNC_SWITCH(strcmp)

typedef size_t strlen_func(const char* __s);
START_IFUNC_SWITCH(strlen)
    // arch-arm/cortex-a9/bionic/strlen.S
    case CORTEX_A9: RETURN_FUNC(strlen_func, strlen_a9);
    // arch-arm/cortex-a15/bionic/strlen.S
    default: RETURN_FUNC(strlen_func, strlen_a15);
END_IFUNC_SWITCH(strlen)


