#include <stddef.h>

extern "C" {

// arch-x86/generic/string/memcmp.S
int memcmp_generic(const void* __lhs, const void* __rhs, size_t __n);

// arch-x86/silvermont/string/sse4-memcmp-slm.S
int memcmp_sse4(const void* __lhs, const void* __rhs, size_t __n);

// arch-x86/atom/string/ssse3-memcmp-atom.S
int memcmp_atom(const void* __lhs, const void* __rhs, size_t __n);

typedef int (*memcmp_ptr)(const void* __lhs, const void* __rhs, size_t __n);
memcmp_ptr memcmp_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) {
        return memcmp_atom;
    } else if (__builtin_cpu_supports("sse4.1")) {
        return memcmp_sse4;
    }
    return memcmp_generic;
}
int memcmp(const void* __lhs, const void* __rhs, size_t __n)
    __attribute__((ifunc("memcmp_resolver")));


// arch-x86/silvermont/string/sse2-memset-slm.S
void* memset_generic(void* __dst, int __ch, size_t __n);

// arch-x86/atom/string/sse2-memset-atom.S
void* memset_atom(void* __dst, int __ch, size_t __n);

typedef void* (*memset_ptr)(void* __dst, int __ch, size_t __n);
memset_ptr memset_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) {
        return memset_atom;
    }
    return memset_generic;
}
void* memset(void* __dst, int __ch, size_t __n)
    __attribute__((ifunc("memset_resolver")));


// arch-x86/silvermont/string/sse2-memset-slm.S
void* __memset_chk_generic(void *s, int c, size_t n, size_t n2);

// arch-x86/atom/string/sse2-memset-atom.S
void* __memset_chk_atom(void *s, int c, size_t n, size_t n2);

typedef void* (*__memset_chk_ptr)(void *s, int c, size_t n, size_t n2);
__memset_chk_ptr __memset_chk_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) {
        return __memset_chk_atom;
    }
    return __memset_chk_generic;
}
void* __memset_chk(void *s, int c, size_t n, size_t n2)
    __attribute__((ifunc("__memset_chk_resolver")));

// arch-x86/silvermont/string/sse2-strlen-slm.S
size_t strlen_generic(const char* __s);

// arch-x86/atom/string/sse2-strlen-atom.S
size_t strlen_atom(const char* __s);
typedef size_t (*strlen_ptr)(const char* __s);
strlen_ptr strlen_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_is("atom")) {
        return strlen_atom;
    }
    return strlen_generic;
}
size_t strlen(const char* __s)
    __attribute__((ifunc("strlen_resolver")));

// upstream-freebsd/lib/libc/string/wmemcmp.c
int wmemcmp_freebsd(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);

// arch-x86/silvermont/string/sse4-wmemcmp-slm.S
int wmemcmp_sse4(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);

typedef int (*wmemcmp_ptr)(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n);
wmemcmp_ptr wmemcmp_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("sse4.1")) {
        return wmemcmp_sse4;
    }
    return wmemcmp_freebsd;
}
int wmemcmp(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n)
    __attribute__((ifunc("wmemcmp_resolver")));


// arch-x86/generic/string/strcmp.S
int strcmp_generic(const char* __lhs, const char* __rhs);

// arch-x86/atom/string/ssse3-strcmp-atom.S
int strcmp_ssse3(const char* __lhs, const char* __rhs);

typedef int (*strcmp_ptr)(const char* __lhs, const char* __rhs);
strcmp_ptr strcmp_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strcmp_ssse3;
    }
    return strcmp_generic;
}
int strcmp(const wchar_t* __lhs, const wchar_t* __rhs, size_t __n)
    __attribute__((ifunc("strcmp_resolver")));


// arch-x86/generic/string/strncmp.S
int strncmp_generic(const char* __lhs, const char* __rhs, size_t __n);

// arch-x86/atom/string/ssse3-strncmp-ato
int strncmp_ssse3(const char* __lhs, const char* __rhs, size_t __n);

typedef int (*strncmp_ptr)(const char* __lhs, const char* __rhs, size_t __n);
strncmp_ptr strncmp_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strncmp_ssse3;
    }
    return strncmp_generic;
}
int strncmp(const char* __lhs, const char* __rhs, size_t __n)
    __attribute__((ifunc("strncmp_resolver")));


// arch-x86/generic/string/strcat.S
char* strcat_generic(char* __dst, const char* __src);

// arch-x86/atom/string/ssse3-strcat-atom.S
char* strcat_ssse3(char* __dst, const char* __src);

typedef char* (*strcat_ptr)(char* __dst, const char* __src);
strcat_ptr strcat_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strcat_ssse3;
    }
    return strcat_generic;
}
char* strcat(char* __dst, const char* __src)
    __attribute__((ifunc("strcat_resolver")));


// upstream-openbsd/lib/libc/string/strncat.c
char* strncat_openbsd(char* __dst, const char* __src, size_t __n);

// arch-x86/atom/string/ssse3-strncat-atom.S
char* strncat_ssse3(char* __dst, const char* __src, size_t __n);

typedef char* (*strncat_ptr)(char* __dst, const char* __src, size_t __n);
strncat_ptr strncat_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strncat_ssse3;
    }
    return strncat_openbsd;
}
char* strncat(char* __dst, const char* __src, size_t __n)
    __attribute__((ifunc("strncat_resolver")));


// upstream-openbsd/lib/libc/string/strlcat.c
size_t strlcat_openbsd(char *dst, const char *src, size_t dsize);

// arch-x86/atom/string/ssse3-strlcat-atom.S
size_t strlcat_ssse3(char *dst, const char *src, size_t dsize);

typedef size_t (*strlcat_ptr)(char *dst, const char *src, size_t dsize);
strlcat_ptr strlcat_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strlcat_ssse3;
    }
    return strlcat_openbsd;
}
size_t strlcat(char* dst, const char* src, size_t size)
    __attribute__((ifunc("strlcat_resolver")));

// upstream-openbsd/lib/libc/string/strlcpy.c
size_t strlcpy_openbsd(char *dst, const char *src, size_t dsize);

// arch-x86/atom/string/ssse3-strlcpy-atom.S
size_t strlcpy_ssse3(char *dst, const char *src, size_t dsize);

typedef size_t (*strlcpy_ptr)(char *dst, const char *src, size_t dsize);
strlcpy_ptr strlcpy_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return strlcpy_ssse3;
    }
    return strlcpy_openbsd;
}
size_t strlcpy(char *dst, const char *src, size_t dsize)
    __attribute__((ifunc("strlcpy_resolver")));


// upstream-freebsd/lib/libc/string/wcscat.c
wchar_t *wcscat_freebsd(wchar_t *s1, const wchar_t *s2);

// arch-x86/atom/string/ssse3-wcscat-atom.S
wchar_t *wcscat_ssse3(wchar_t *s1, const wchar_t *s2);

typedef wchar_t* (*wcscat_ptr)(wchar_t *s1, const wchar_t *s2);
wcscat_ptr wcscat_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return wcscat_ssse3;
    }
    return wcscat_freebsd;
}
wchar_t wcscat(wchar_t *s1, const wchar_t *s2)
    __attribute__((ifunc("wcscat_resolver")));


// upstream-freebsd/lib/libc/string/wcscpy.c
wchar_t *wcscpy_freebsd(wchar_t *s1, const wchar_t *s2);

// arch-x86/atom/string/ssse3-wcscpy-atom.S
wchar_t *wcscpy_ssse3(wchar_t *s1, const wchar_t *s2);

typedef wchar_t* (*wcscpy_ptr)(wchar_t *s1, const wchar_t *s2);
wcscpy_ptr wcscpy_resolver() {
    __builtin_cpu_init();
    if (__builtin_cpu_supports("ssse3")) {
        return wcscpy_ssse3;
    }
    return wcscpy_freebsd;
}
wchar_t *wcscpy(wchar_t *s1, const wchar_t *s2)
    __attribute__((ifunc("wcscpy_resolver")));

}
