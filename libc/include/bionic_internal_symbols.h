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
// No amount of token manipulation (e.g. f#oo) helps here.
// As soon as a token foo materializes, it is #defined to become __bionic_foo.
// So the best way is to #undef foo.
// Refer to bionic_external_symbols.h

// This is somewhat convoluted, but I believe this is better than
// touching every call site and amend each call to
// explicitly reference __bionic_foo().

// The patch impact are two new #includes per .cpp file and two
// wholly separate files

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

// PTHREAD_CREATE
#define pthread_create INTERNAL(pthread_create)

// PTHREAD_JOIN
#define pthread_join INTERNAL(pthread_join)

// MISC_BIONIC_ROUTINES
#define memset INTERNAL(memset)
#define open INTERNAL(open)
#define openat INTERNAL(openat)

#define mmap INTERNAL(mmap)
#define mmap64 INTERNAL(mmap64)
#define munmap INTERNAL(munmap)
#define mprotect INTERNAL(mprotect)
#if 0
#define msync INTERNAL(msync)
#define mremap INTERNAL(mremap)
#define mlockall INTERNAL(mlockall)
#define munlockall INTERNAL(munlockall)
#define mlock INTERNAL(mlock)
#define munlock INTERNAL(munlock)
#define madvise INTERNAL(madvise)
#define mincore INTERNAL(mincore)
#endif
#define posix_madvise INTERNAL(posix_madvise)

// These are the calls with internal symbol duplicates

// pthread_mutex.cpp
#ifdef _PTHREAD_H_
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

// pthread_join.cpp
DECLARE_INTERNAL(int, pthread_join, pthread_t t, void** return_value);
#endif

// MALLOC_DEBUG_COMMON
// Unlike others, there are lots of symbol clashes for malloc/free et.al..
// So instead of relying on more fragile macro magic, we explicitly change the
// call sites to use INTERNAL(malloc/free/...)
// Note that malloc_debug_common.cpp still #includes bionic_external_symbols.h

DECLARE_INTERNAL(int, open, const char*, int, ...);
DECLARE_INTERNAL(int, openat, int, const char*, int, ...);

// get memset in here
// memset is conditionally declared as an inline function
// in bionic/include/string.h
#ifndef __BIONIC_FORTIFY_INLINE
DECLARE_INTERNAL(void *, memset, void *s, int c, size_t n);
#endif

// MMAP_CPP
#ifdef _SYS_MMAN_H_
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

#if 0
#endif


#endif // _BIONIC_INTERNAL_SYMBOLS_H_
