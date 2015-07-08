#ifndef _BIONIC_INTERNAL_SYMBOLS_H_
#define _BIONIC_INTERNAL_SYMBOLS_H_

// These macro serves two purposes.

// It is used during function definition time to set up a
// private alias for the function body of the name __bionic_foo()
// which is a weak alias for the function foo().

// The function foo() is the publically available entry point that the
// applications can use. __bionic_foo() is a private entry to foo() that
// bionic itself uses.
// This is mostly to support Thread Sanitizer. It currently intercepts
// internal bionic-bionic calls that creates hazards between TSAN and BIONIC.

// In order to reduce the amount of changes to the source code,
// we define foo() as __bionic_foo().

// This presents a problem:
// DEFINE_EXTERNAL(foo)(..) aliases foo() to __bionic_foo(),
// but since foo has already been #defined to __bionic_foo, we first have
// to undefine foo, and then declare foo() as an alias to __bionic_foo().
// Refer to bionic_external_symbols.h

// This is somewhat convoluted, but I believe this is better than
// touching every call site and amend each call to
// explicitly reference __bionic_foo().

// The patch impact are two new #includes per .cpp file and two
// wholly separate files that are included.

#undef INTERNAL
#undef DEFINE_EXTERNAL
#undef DECLARE_INTERNAL

#define INTERNAL(FNAME) __bionic_##FNAME

#if defined(__cplusplus)
#define EXTERN_C_DECL extern "C"
#else
#define EXTERN_C_DECL extern
#endif

#define DEFINE_EXTERNAL(RTNTYPE, FNAME, ...) \
  EXTERN_C_DECL RTNTYPE FNAME(__VA_ARGS__)                                      \
  __attribute__((weak, alias("__bionic_"#FNAME), visibility("default")))

#define DECLARE_INTERNAL(RTNTYPE, FNAME, ...) \
   EXTERN_C_DECL __attribute__((visibility("default"))) RTNTYPE __bionic_##FNAME(__VA_ARGS__)

#endif // _BIONIC_INTERNAL_SYMBOLS_H_
// Yes thats right. _BIONIC_INTERNAL_SYMBOLS_H_ ends here!!
// The rest of this file can get included multiple times
// from the various .h files that declare the routines below.
// Note that they use the same guards as the header files
// that originally declared the routines.

// These are the calls with internal symbol duplicates

// pthread_mutex.cpp
#ifdef _PTHREAD_H_
// PTHREAD_MUTEX
#define pthread_mutexattr_init INTERNAL(pthread_mutexattr_init)
#define pthread_mutexattr_destroy INTERNAL(pthread_mutexattr_destroy)
#define pthread_mutexattr_gettype INTERNAL(pthread_mutexattr_gettype)
#define pthread_mutexattr_settype INTERNAL(pthread_mutexattr_settype)
#define pthread_mutexattr_setpshared INTERNAL(pthread_mutexattr_setpshared)
#define pthread_mutexattr_getpshared INTERNAL(pthread_mutexattr_getpshared)
#define pthread_mutex_init INTERNAL(pthread_mutex_init)
#define pthread_mutex_lock INTERNAL(pthread_mutex_lock)
#define pthread_mutex_unlock INTERNAL(pthread_mutex_unlock)
#define pthread_mutex_trylock INTERNAL(pthread_mutex_trylock)
#define pthread_mutex_timedlock INTERNAL(pthread_mutex_timedlock)
#define pthread_mutex_destroy INTERNAL(pthread_mutex_destroy)


#define pthread_once INTERNAL(pthread_once)

// PTHREAD_CREATE
#define pthread_create INTERNAL(pthread_create)

// PTHREAD_JOIN
#define pthread_join INTERNAL(pthread_join)


DECLARE_INTERNAL(int, pthread_mutexattr_init, pthread_mutexattr_t *attr);
DECLARE_INTERNAL(int, pthread_mutexattr_destroy, pthread_mutexattr_t *attr);
DECLARE_INTERNAL(int, pthread_mutexattr_gettype, const pthread_mutexattr_t *attr, int *type_p);
DECLARE_INTERNAL(int, pthread_mutexattr_settype, pthread_mutexattr_t *attr, int type);
DECLARE_INTERNAL(int, pthread_mutexattr_setpshared, pthread_mutexattr_t *attr, int  pshared);
DECLARE_INTERNAL(int, pthread_mutexattr_getpshared, const pthread_mutexattr_t* attr, int* pshared);
DECLARE_INTERNAL(int, pthread_mutex_init, pthread_mutex_t* mutex_interface, const pthread_mutexattr_t* attr);
DECLARE_INTERNAL(int, pthread_mutex_lock, pthread_mutex_t* mutex_interface);
DECLARE_INTERNAL(int, pthread_mutex_unlock, pthread_mutex_t* mutex_interface);
DECLARE_INTERNAL(int, pthread_mutex_trylock, pthread_mutex_t* mutex_interface);
DECLARE_INTERNAL(int, pthread_mutex_timedlock, pthread_mutex_t* mutex_interface, const struct timespec* abs_timeout);
DECLARE_INTERNAL(int, pthread_mutex_destroy, pthread_mutex_t* mutex_interface);

// pthread_create.cpp
DECLARE_INTERNAL(int, pthread_create, pthread_t* thread_out, pthread_attr_t const* attr, \
                 void* (*start_routine)(void*), void* arg);

// pthread_once.cpp

DECLARE_INTERNAL(int, pthread_once, pthread_once_t* once_control, void (*init_routine)(void));
// pthread_join.cpp
DECLARE_INTERNAL(int, pthread_join, pthread_t t, void** return_value);
#endif

// MALLOC_DEBUG_COMMON
// Unlike others, there are lots of symbol clashes for malloc/free et.al..
// So instead of relying on more fragile macro magic, we explicitly change the
// call sites to use INTERNAL(malloc/free/...)
// Note that malloc_debug_common.cpp still #includes bionic_external_symbols.h

#if defined(_FCNTL_H)
#define open INTERNAL(open)
#define openat INTERNAL(openat)

DECLARE_INTERNAL(int, open, const char*, int, ...);
DECLARE_INTERNAL(int, openat, int, const char*, int, ...);
#endif


#if defined(_STRING_H)
// string.h --
// get memset and friends in here
// Note that we don't #define memset here.
// memset and a few others are conditionally declared as an inline function
// in bionic/include/string.h, so we do it there
#define memccpy INTERNAL(memccpy)
#define memcmp INTERNAL(memcmp)
#if defined(__USE_GNU)
#define mempcpy INTERNAL(mempcpy)
#endif

#define memmem INTERNAL(memmem)

#define strchr INTERNAL(strchr)

#ifndef __BIONIC_FORTIFY
#define memchr INTERNAL(memchr)
#define memrchr INTERNAL(memrchr)

#define memcpy INTERNAL(memcpy)
#define memmove INTERNAL(memmove)

#define stpcpy INTERNAL(stpcpy)
#define strcpy INTERNAL(strcpy)
#define stpncpy INTERNAL(stpncpy)
#define strncpy INTERNAL(strncpy)
#define strcat INTERNAL(strcat)
#define strncat INTERNAL(strncat)
#define memset INTERNAL(memset)

#define strlcpy INTERNAL(strlcpy)
#define strlcat INTERNAL(strlcat)

#define strlen INTERNAL(strlen)
#define strrchr INTERNAL(strrchr)
#endif

#define strcmp INTERNAL(strcmp)

#define strcasecmp INTERNAL(strcasecmp)
#define strcasecmp_l INTERNAL(strcasecmp_l)
#define strncasecmp INTERNAL(strncasecmp)
#define strncasecmp_l INTERNAL(strncasecmp_l)

#define strdup INTERNAL(strdup)

#define strstr INTERNAL(strstr)
#define strcasestr INTERNAL(strcasestr)
#define strtok INTERNAL(strtok)
#define strtok_r INTERNAL(strtok_r)

#define strerror INTERNAL(strerror)
#define strerror_l INTERNAL(strerror_l)
//#define strerror_r INTERNAL(strerror_r)

#define strnlen INTERNAL(strnlen)
#define strndup INTERNAL(strndup)
#define strncmp INTERNAL(strncmp)

#define strcspn INTERNAL(strcspn)
#define strpbrk INTERNAL(strpbrk)
#define strsep INTERNAL(strsep)
#define strspn INTERNAL(strspn)
#define strsignal INTERNAL(strsignal)
#define strcoll INTERNAL(strcoll)
#define strxfrm INTERNAL(strxfrm)

#define strcoll_l INTERNAL(strcoll_l)
#define strxfrm_l INTERNAL(strxfrm_l)

DECLARE_INTERNAL(void*,  memccpy, void* __restrict, const void* __restrict, int, size_t);
DECLARE_INTERNAL(void*,  memchr, const void *, int, size_t) /*__purefunc*/;
DECLARE_INTERNAL(void*,  memrchr, const void *, int, size_t) /*__purefunc*/;
DECLARE_INTERNAL(int,    memcmp, const void *, const void *, size_t) /*__purefunc*/;
DECLARE_INTERNAL(void*,  memcpy, void* __restrict, const void* __restrict, size_t);
#if defined(__USE_GNU)
DECLARE_INTERNAL(void*,  mempcpy, void* __restrict, const void* __restrict, size_t);
#endif
DECLARE_INTERNAL(void*,  memmove, void *, const void *, size_t);
DECLARE_INTERNAL(void*,  memset, void *, int, size_t);
DECLARE_INTERNAL(void*,  memmem, const void *, size_t, const void *, size_t) /*__purefunc*/;

DECLARE_INTERNAL(char*,  strchr, const char *, int) /*__purefunc*/;

DECLARE_INTERNAL(char*,  strrchr, const char *, int) /*__purefunc*/;

DECLARE_INTERNAL(size_t, strlen, const char *) /*__purefunc*/;
DECLARE_INTERNAL(int,    strcmp, const char *, const char *) /*__purefunc*/;
DECLARE_INTERNAL(char*,  stpcpy, char* __restrict, const char* __restrict);
DECLARE_INTERNAL(char*,  strcpy, char* __restrict, const char* __restrict);
DECLARE_INTERNAL(char*,  strcat, char* __restrict, const char* __restrict);

DECLARE_INTERNAL(int, strcasecmp, const char*, const char*) /*__purefunc*/;
DECLARE_INTERNAL(int, strcasecmp_l, const char*, const char*, locale_t) /*__purefunc*/;
DECLARE_INTERNAL(int, strncasecmp, const char*, const char*, size_t) /*__purefunc*/;
DECLARE_INTERNAL(int, strncasecmp_l, const char*, const char*, size_t, locale_t) /*__purefunc*/;

DECLARE_INTERNAL(char*,  strdup, const char *);

DECLARE_INTERNAL(char*,  strstr, const char *, const char *) /*__purefunc*/;
DECLARE_INTERNAL(char*,  strcasestr, const char *haystack, const char *needle) /*__purefunc*/;
DECLARE_INTERNAL(char*,  strtok, char* __restrict, const char* __restrict);
DECLARE_INTERNAL(char*,  strtok_r, char* __restrict, const char* __restrict, char** __restrict);

DECLARE_INTERNAL(char*, strerror, int);
DECLARE_INTERNAL(char*, strerror_l, int, locale_t);
// #if defined(__USE_GNU)
// DECLARE_INTERNAL(char*, strerror_r, int, char*, size_t) __RENAME(__gnu_strerror_r);
// #else /* POSIX */
// DECLARE_INTERNAL(int, strerror_r, int, char*, size_t);
// #endif

DECLARE_INTERNAL(size_t, strnlen, const char *, size_t) /*__purefunc*/;
DECLARE_INTERNAL(char*,  strncat, char* __restrict, const char* __restrict, size_t);
DECLARE_INTERNAL(char*,  strndup, const char *, size_t);
DECLARE_INTERNAL(int,    strncmp, const char *, const char *, size_t) /*__purefunc*/;
DECLARE_INTERNAL(char*,  stpncpy, char* __restrict, const char* __restrict, size_t);
DECLARE_INTERNAL(char*,  strncpy, char* __restrict, const char* __restrict, size_t);

DECLARE_INTERNAL(size_t, strlcat, char* __restrict, const char* __restrict, size_t);
DECLARE_INTERNAL(size_t, strlcpy, char* __restrict, const char* __restrict, size_t);

DECLARE_INTERNAL(size_t, strcspn, const char *, const char *) /*__purefunc*/;
DECLARE_INTERNAL(char*,  strpbrk, const char *, const char *) /*__purefunc*/;
DECLARE_INTERNAL(char*,  strsep, char** __restrict, const char* __restrict);
DECLARE_INTERNAL(size_t, strspn, const char *, const char *);

DECLARE_INTERNAL(char*,  strsignal, int  sig);

DECLARE_INTERNAL(int,    strcoll, const char *, const char *) /*__purefunc*/;
DECLARE_INTERNAL(size_t, strxfrm, char* __restrict, const char* __restrict, size_t);

DECLARE_INTERNAL(int,    strcoll_l, const char *, const char *, locale_t) /*__purefunc*/;
DECLARE_INTERNAL(size_t, strxfrm_l, char* __restrict, const char* __restrict, size_t, locale_t);

#if 0
#if defined(__USE_GNU) && !defined(__bionic_using_posix_basename)
/*
 * glibc has a basename in <string.h> that's different to the POSIX one in <libgen.h>.
 * It doesn't modify its argument, and in C++ it's const-correct.
 */
#if defined(__cplusplus)
extern "C++" char* basename(char*) __RENAME(__gnu_basename) __nonnull((1));
extern "C++" const char* basename(const char*) __RENAME(__gnu_basename) __nonnull((1));
#else
extern char* basename(const char*) __RENAME(__gnu_basename) __nonnull((1));
#endif
#endif
#endif
#endif

// MMAP_CPP
#ifdef _SYS_MMAN_H_
#define mmap INTERNAL(mmap)
#define mmap64 INTERNAL(mmap64)
#define munmap INTERNAL(munmap)
#define mprotect INTERNAL(mprotect)
#define msync INTERNAL(msync)
#define mremap INTERNAL(mremap)
#define mlockall INTERNAL(mlockall)
#define munlockall INTERNAL(munlockall)
#define mlock INTERNAL(mlock)
#define munlock INTERNAL(munlock)
#define madvise INTERNAL(madvise)
#define mincore INTERNAL(mincore)
#define posix_madvise INTERNAL(posix_madvise)

DECLARE_INTERNAL(void*, mmap, void* addr, size_t size, int prot, int flags, int fd, off_t offset);
DECLARE_INTERNAL(void*, mmap64, void* addr, size_t size, int prot, int flags, int fd, off64_t offset);
DECLARE_INTERNAL(int, munmap, void*, size_t);
DECLARE_INTERNAL(int, mprotect, const void*, size_t, int);

DECLARE_INTERNAL(int, msync, const void*, size_t, int);
DECLARE_INTERNAL(void*, mremap, void*, size_t, size_t, unsigned long);
DECLARE_INTERNAL(int, mlockall, int);
DECLARE_INTERNAL(int, munlockall, void);
DECLARE_INTERNAL(int, mlock, const void*, size_t);
DECLARE_INTERNAL(int, munlock, const void*, size_t);
DECLARE_INTERNAL(int, madvise, void*, size_t, int);
DECLARE_INTERNAL(int, mincore, void*, size_t, unsigned char*);

DECLARE_INTERNAL(int, posix_madvise, void*, size_t, int);
#endif

#ifdef _SYS_PRCTL_H
#define prctl INTERNAL(prctl)

DECLARE_INTERNAL(int, prctl, int option, ...);
#endif


#if defined(LIBC_INCLUDE_MALLOC_H_)||defined(LIBC_BIONIC_DLMALLOC_H_)
//  && !defined(__IN_BIONIC_LINKER__) && \
//  !defined(DLMALLOC_VERSION)&&!defined(MALLOC_280_H) &&!defined(LIBC_BIONIC_DLMALLOC_H_)

// #define memalign INTERNAL(memalign)
// #define malloc_usable_size INTERNAL(malloc_usable_size)
// #define mallinfo INTERNAL(mallinfo)
//ignore malloc_info for now
//#define malloc_info INTERNAL(malloc_info)


DECLARE_INTERNAL(void*, malloc, size_t byte_count) /*__mallocfunc __wur __attribute__((alloc_size(1)))*/;
DECLARE_INTERNAL(void*, calloc, size_t item_count, size_t item_size) /*__mallocfunc __wur __attribute__((alloc_size(1,2)))*/;
DECLARE_INTERNAL(void*, realloc, void* p, size_t byte_count) /*__wur __attribute__((alloc_size(2)))*/;
DECLARE_INTERNAL(void, free, void* p);

#define malloc INTERNAL(malloc)
#define calloc INTERNAL(calloc)
#define realloc INTERNAL(realloc)
#define free INTERNAL(free)

// DECLARE_INTERNAL(void*, memalign, size_t alignment, size_t byte_count) /*__mallocfunc __wur __attribute__((alloc_size(2)))*/;
// DECLARE_INTERNAL(size_t, malloc_usable_size, const void* p);
// DECLARE_INTERNAL(struct mallinfo, mallinfo, void);
//DECLARE_INTERNAL(int, malloc_info, int, FILE *);

#endif

// #if defined(MALLOC_280_H) && !defined(__LINKER_ALLOCATOR_H)
// #define mallinfo INTERNAL(mallinfo)
// #endif

#ifdef _SYS_IOCTL_H_
#define ioctl INTERNAL(ioctl)
#define __ioctl INTERNAL(__ioctl)
DECLARE_INTERNAL(int, ioctl, int, int, ...);
DECLARE_INTERNAL(int, __ioctl, int, int, void *);
#endif

#ifdef _TERMIOS_H_
#if __ANDROID_API__ >= 21
DECLARE_INTERNAL(void , cfmakeraw,   struct termios*);
DECLARE_INTERNAL(int ,  cfsetispeed, struct termios*, speed_t);
DECLARE_INTERNAL(int ,  cfsetospeed, struct termios*, speed_t);
DECLARE_INTERNAL(int ,  cfsetspeed,  struct termios*, speed_t);
DECLARE_INTERNAL(int ,  tcdrain,     int);
DECLARE_INTERNAL(int ,  tcflow,      int, int);
DECLARE_INTERNAL(int ,  tcflush,     int, int);
DECLARE_INTERNAL(int ,  tcgetattr,   int, struct termios*);
//DECLARE_INTERNAL(pid_t, tcgetsid,    int);
DECLARE_INTERNAL(int ,  tcsendbreak, int, int);
DECLARE_INTERNAL(int ,  tcsetattr,   int, int, const struct termios*);

#define cfmakeraw INTERNAL(cfmakeraw)
#define cfsetispeed INTERNAL(cfsetispeed)
#define cfsetospeed INTERNAL(cfsetospeed)
#define cfsetspeed INTERNAL(cfsetspeed)
#define tcdrain INTERNAL(tcdrain)
#define tcflow INTERNAL(tcflow)
#define tcflush INTERNAL(tcflush)
#define tcgetattr INTERNAL(tcgetattr)
//#define tcgetsid INTERNAL(tcgetsid)
#define tcsendbreak INTERNAL(tcsendbreak)
#define tcsetattr INTERNAL(tcsetattr)

#endif
#endif
