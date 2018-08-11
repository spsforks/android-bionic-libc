#include <stddef.h>

struct __processor_model {
    unsigned int __cpu_vendor;
    unsigned int __cpu_type;
    unsigned int __cpu_subtype;
    unsigned int __cpu_features[1];
};

__attribute__((visibility("hidden")))
extern "C" __processor_model __cpu_model;

// These definitions have to match the values in
// llvm/include/llvm/Support/X86TargetParser.def
const int SSSE3  = 6;
const int SSE4_1 = 7;
const int ATOM   = 1;

// __builtin_cpu_supports and __builtin_cpu_is can not be used here. They
// don't access __cpu_model directly but use GOT.
static bool inline cpu_supports(unsigned int feature) {
    return (__cpu_model.__cpu_features[0] & (1U << feature)) != 0;
}

static bool inline cpu_is(unsigned int type) {
    return (__cpu_model.__cpu_type == type);
}

#define DEFINE_IFUNC_FOR(name) \
    name##_func name __attribute__((ifunc(#name "_resolver"))); \
    __attribute__((visibility("hidden"))) \
    name##_func* name##_resolver()

#define IFUNC_CONDITION(cond, type, name) \
    __attribute__((visibility("hidden"))) \
    type name; \
    if (cond) return name;

extern "C" {

typedef int memcmp_func(const void* __lhs, const void* __rhs, size_t __n);
DEFINE_IFUNC_FOR(memcmp) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-memcmp-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), memcmp_func, memcmp_atom);

    // arch-x86/silvermont/string/sse4-memcmp-slm.S
    IFUNC_CONDITION(cpu_supports(SSE4_1), memcmp_func, memcmp_sse4);

    // arch-x86/generic/string/memcmp.S
    IFUNC_CONDITION(true, memcmp_func, memcmp_generic);
}

typedef void* memset_func(void* __dst, int __ch, size_t __n);
DEFINE_IFUNC_FOR(memset) {
    __builtin_cpu_init();

    // arch-x86/atom/string/sse2-memset-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), memset_func, memset_atom);

    // arch-x86/silvermont/string/sse2-memset-slm.S
    IFUNC_CONDITION(true, memset_func, memset_generic);
}

typedef void* __memset_chk_func(void *s, int c, size_t n, size_t n2);
DEFINE_IFUNC_FOR(__memset_chk) {
    __builtin_cpu_init();

    // arch-x86/atom/string/sse2-memset-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), __memset_chk_func, __memset_chk_atom);

    // arch-x86/silvermont/string/sse2-memset-slm.S
    IFUNC_CONDITION(true, __memset_chk_func, __memset_chk_generic);
}

typedef void* memcpy_func(void*, const void*, size_t);
DEFINE_IFUNC_FOR(memcpy) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-memcpy-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), memcpy_func, memcpy_atom);

    // arch-x86/silvermont/string/sse2-memmove-slm.S
    IFUNC_CONDITION(true, memcpy_func, memcpy_generic);
}

typedef void* memmove_func(void* __dst, const void* __src, size_t __n);
DEFINE_IFUNC_FOR(memmove) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-memcpy-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), memmove_func, memmove_atom);

    // arch-x86/silvermont/string/sse2-memmove-slm.S
    IFUNC_CONDITION(true, memmove_func, memmove_generic);
}

typedef char* strcpy_func(char* __dst, const char* __src);
DEFINE_IFUNC_FOR(strcpy) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strcpy-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), strcpy_func, strcpy_atom);

    // arch-x86/silvermont/string/sse2-memmove-slm.S
    IFUNC_CONDITION(true, strcpy_func, strcpy_generic);
}

typedef char* strncpy_func(char* __dst, const char* __src, size_t __n);
DEFINE_IFUNC_FOR(strncpy) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strcpy-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), strncpy_func, strncpy_atom);

    // arch-x86/silvermont/string/sse2-memmove-slm.S
    IFUNC_CONDITION(true, strncpy_func, strncpy_generic);
}

typedef size_t strlen_func(const char* __s);
DEFINE_IFUNC_FOR(strlen) {
    __builtin_cpu_init();

    // arch-x86/atom/string/sse2-strlen-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), strlen_func, strlen_atom);

    // arch-x86/silvermont/string/sse2-strlen-slm.S
    IFUNC_CONDITION(true, strlen_func, strlen_generic);
}

typedef int wmemcmp_func(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);
DEFINE_IFUNC_FOR(wmemcmp) {
    __builtin_cpu_init();

    // arch-x86/silvermont/string/sse4-wmemcmp-slm.S
    IFUNC_CONDITION(cpu_supports(SSE4_1), wmemcmp_func, wmemcmp_sse4);

    // arch-x86/atom/string/ssse3-wmemcmp-atom.S
    IFUNC_CONDITION(cpu_is(ATOM), wmemcmp_func, wmemcmp_atom);

    // upstream-freebsd/lib/libc/string/wmemcmp.c
    IFUNC_CONDITION(true, wmemcmp_func, wmemcmp_freebsd);
}

typedef int strcmp_func(const char* __lhs, const char* __rhs);
DEFINE_IFUNC_FOR(strcmp) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strcmp-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), strcmp_func, strcmp_ssse3);

    // arch-x86/generic/string/strcmp.S
    IFUNC_CONDITION(true, strcmp_func, strcmp_generic);
}

typedef int strncmp_func(const char* __lhs, const char* __rhs, size_t __n);
DEFINE_IFUNC_FOR(strncmp) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strncmp-ato
    IFUNC_CONDITION(cpu_supports(SSSE3), strncmp_func, strncmp_ssse3);

    // arch-x86/generic/string/strncmp.S
    IFUNC_CONDITION(true, strncmp_func, strncmp_generic);
}

typedef char* strcat_func(char* __dst, const char* __src);
DEFINE_IFUNC_FOR(strcat) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strcat-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), strcat_func, strcat_ssse3);

    // arch-x86/generic/string/strcat.S
    IFUNC_CONDITION(true, strcat_func, strcat_generic);
}

typedef char* strncat_func(char* __dst, const char* __src, size_t __n);
DEFINE_IFUNC_FOR(strncat) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strncat-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), strncat_func, strncat_ssse3);

    // upstream-openbsd/lib/libc/string/strncat.c
    IFUNC_CONDITION(true, strncat_func, strncat_openbsd);
}

typedef size_t strlcat_func(char *dst, const char *src, size_t dsize);
DEFINE_IFUNC_FOR(strlcat) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strlcat-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), strlcat_func, strlcat_ssse3);

    // upstream-openbsd/lib/libc/string/strlcat.c
    IFUNC_CONDITION(true, strlcat_func, strlcat_openbsd);
}

typedef size_t strlcpy_func(char *dst, const char *src, size_t dsize);
DEFINE_IFUNC_FOR(strlcpy) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-strlcpy-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), strlcpy_func, strlcpy_ssse3);

    // upstream-openbsd/lib/libc/string/strlcpy.c
    IFUNC_CONDITION(true, strlcpy_func, strlcpy_openbsd);
}

typedef wchar_t* wcscat_func(wchar_t *s1, const wchar_t *s2);
DEFINE_IFUNC_FOR(wcscat) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-wcscat-atom.S
    IFUNC_CONDITION(cpu_supports(SSSE3), wcscat_func, wcscat_ssse3);

    // upstream-freebsd/lib/libc/string/wcscat.c
    IFUNC_CONDITION(true, wcscat_func, wcscat_freebsd);
}

typedef wchar_t* wcscpy_func(wchar_t *s1, const wchar_t *s2);
DEFINE_IFUNC_FOR(wcscpy) {
    __builtin_cpu_init();

    // arch-x86/atom/string/ssse3-wcscpy-atom.S
    // if (__cpu_model.__cpu_features[0] & 0x40) {
    IFUNC_CONDITION(cpu_supports(SSSE3), wcscpy_func, wcscpy_ssse3);

    // upstream-freebsd/lib/libc/string/wcscpy.c
    IFUNC_CONDITION(true, wcscpy_func, wcscpy_freebsd);
}

}
