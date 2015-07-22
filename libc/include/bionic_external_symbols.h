// WARNING!
// This file has to be #included multiple times!
// Please read the comments in bionic_internal_symbols.h first!

// We guard DEFINE_EXTERNAL() calls below.
// First, we undefine all of the foo's
//  (which have been defined earlier as __bionic_foo!)
// then we selectively define some external symbols that correspond to foo

// The result is that __bionic_foo() has been defined, and all the calls to
// foo() has been switched out to call  __bionic_foo() instead.
// and also foo() becomes a weak alias for __bionic_foo.
// Therefore, TSAN (and the like) can no longer intercept calls made inside
// bionic

#undef pthread_mutexattr_init
#undef pthread_mutexattr_destroy
#undef pthread_mutexattr_gettype
#undef pthread_mutexattr_settype
#undef pthread_mutexattr_setpshared
#undef pthread_mutexattr_getpshared
#undef pthread_mutex_init
#undef pthread_mutex_lock
#undef pthread_mutex_unlock
#undef pthread_mutex_trylock
#undef pthread_mutex_timedlock
#undef pthread_mutex_destroy

#ifdef PTHREAD_MUTEX_EXTERNALS
DEFINE_EXTERNAL(int, pthread_mutexattr_init, pthread_mutexattr_t *attr);
DEFINE_EXTERNAL(int, pthread_mutexattr_destroy, pthread_mutexattr_t *attr);
DEFINE_EXTERNAL(int, pthread_mutexattr_gettype, const pthread_mutexattr_t *attr, int *type_p);
DEFINE_EXTERNAL(int, pthread_mutexattr_settype, pthread_mutexattr_t *attr, int type);
DEFINE_EXTERNAL(int, pthread_mutexattr_setpshared, pthread_mutexattr_t *attr, int  pshared);
DEFINE_EXTERNAL(int, pthread_mutexattr_getpshared, const pthread_mutexattr_t* attr, int* pshared);
DEFINE_EXTERNAL(int, pthread_mutex_init, pthread_mutex_t* mutex_interface, const pthread_mutexattr_t* attr);
DEFINE_EXTERNAL(int, pthread_mutex_lock, pthread_mutex_t* mutex_interface);
DEFINE_EXTERNAL(int, pthread_mutex_unlock, pthread_mutex_t* mutex_interface);
DEFINE_EXTERNAL(int, pthread_mutex_trylock, pthread_mutex_t* mutex_interface);
DEFINE_EXTERNAL(int, pthread_mutex_timedlock, pthread_mutex_t* mutex_interface, const timespec* abs_timeout);
DEFINE_EXTERNAL(int, pthread_mutex_destroy, pthread_mutex_t* mutex_interface);
#endif

#undef pthread_create

#ifdef PTHREAD_CREATE_EXTERNALS
DEFINE_EXTERNAL(int, pthread_create, pthread_t* thread_out, pthread_attr_t const* attr, \
                void* (*start_routine)(void*), void* arg);
#endif

#undef pthread_join

#ifdef PTHREAD_JOIN_EXTERNALS
DEFINE_EXTERNAL(int, pthread_join, pthread_t t, void** return_value);
#endif

#ifdef MALLOC_DEBUG_COMMON_CPP
#undef calloc
#undef malloc
#undef free
#undef mallinfo
#undef realloc

DEFINE_EXTERNAL(void*, malloc, size_t bytes);
DEFINE_EXTERNAL(void*, calloc, size_t n_elements, size_t elem_size);
DEFINE_EXTERNAL(void*, realloc, void* oldMem, size_t bytes);
DEFINE_EXTERNAL(void, free, void* mem);
//DEFINE_EXTERNAL(struct INTERNAL(mallinfo), mallinfo);
//DEFINE_EXTERNAL(size_t, malloc_usable_size, const void* mem);
//DEFINE_EXTERNAL(void*, memalign, size_t alignment, size_t bytes);
//DEFINE_EXTERNAL(int, posix_memalign, void** memptr, size_t alignment, size_t size);
// #if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
// DEFINE_EXTERNAL(void*, pvalloc, size_t bytes);
// #endif
// #if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
// DEFINE_EXTERNAL(void*, valloc, size_t bytes);
// #endif
#endif

#undef memset
#undef open
#undef openat

// #ifdef MEMSET_C_DEFS
// DEFINE_EXTERNAL(void*, memset, void *s, int c, size_t n);
// #endif

#ifdef OPEN_CPP
DEFINE_EXTERNAL(int, open, const char *pathname, int flags, ...);
DEFINE_EXTERNAL(int, openat, int fd, const char *pathname, int flags, ...);
#endif

#undef mmap
#undef mmap64

#undef munmap
#undef msync
#undef mprotect
#undef mremap
#undef mlockall
#undef munlockall
#undef madvise
#undef mlock
#undef munlock
#undef mincore
#undef posix_madvise


#define CAST_TO(T, Arg) (T)(Arg)


#define MYSYSCALL(RTNTYPE, N, ...)                               \
   return CAST_TO(RTNTYPE, syscall(SYS_##N, __VA_ARGS__))

#ifndef EXTERN_C_DECL
#ifdef __cplusplus
#define EXTERN_C_DECL extern "C"
#else
#define EXTERN_C_DECL extern
#endif
#endif

#if 0
#define DEFINE_SYSCALL_EXTERNAL(RTNTYPE, FNAME, ...) \
  EXTERN_C_DECL RTNTYPE FNAME(__VA_ARGS__) \
  __attribute__((weak, alias("__bionic_"#FNAME), visibility("default"))); \
  EXTERN_C_DECL RTNTYPE __bionic_##FNAME(__VA_ARGS__)
#else
#define DEFINE_SYSCALL_EXTERNAL(RTNTYPE, FNAME, ...) \
  __attribute__((weak, visibility("default"))) EXTERN_C_DECL RTNTYPE __bionic_##FNAME(__VA_ARGS__)
#endif


#if defined(MMAP_CPP)
DEFINE_EXTERNAL(void*, mmap, void* addr, size_t size, int prot, int flags, int fd, off_t offset);
DEFINE_EXTERNAL(void*, mmap64, void* addr, size_t size, int prot, int flags, int fd, off64_t offset);
#endif

#ifdef POSIX_MADVISE_CPP
DEFINE_EXTERNAL(int, posix_madvise, void*, size_t, int);
#endif

#undef memccpy
#undef memchr
#undef memrchr
#undef memcmp
#undef memcpy
#undef mempcpy
#undef memmove
#undef memset
#undef memmem

#undef strchr

#undef strrchr

#undef strlen
#undef strcmp
#undef stpcpy
#undef strcpy
#undef strcat

#undef strcasecmp
#undef strcasecmp_l
#undef strncasecmp
#undef strncasecmp_l

#undef strdup

#undef strstr
#undef strcasestr
#undef strtok
#undef strtok_r

#undef strerror
#undef strerror_l
#undef strerror_r

#undef strnlen
#undef strncat
#undef strndup
#undef strncmp
#undef stpncpy
#undef strncpy

#undef strlcat
#undef strlcpy

#undef strcspn
#undef strpbrk
#undef strsep
#undef strspn
#undef strsignal
#undef strcoll
#undef strxfrm

#undef strcoll_l
#undef strxfrm_l


#ifdef MEMCCPY_C
DEFINE_EXTERNAL(void*,  memccpy, void* __restrict, const void* __restrict, int, size_t);
#endif
#ifdef MEMCHR_C
DEFINE_EXTERNAL(void*,  memchr, const void *, int, size_t) /*__purefunc*/;
#endif
#ifdef MEMRCHR_C
DEFINE_EXTERNAL(void*,  memrchr, const void *, int, size_t) /*__purefunc*/;
#endif

//DEFINE_EXTERNAL(int,    memcmp, const void *, const void *, size_t) /*__purefunc*/;
//DEFINE_EXTERNAL(void*,  memcpy, void* __restrict, const void* __restrict, size_t);
#if defined(__USE_GNU)&&defined(MEMPCPY_CPP)
DEFINE_EXTERNAL(void*,  mempcpy, void* __restrict, const void* __restrict, size_t);
#endif
#if defined(MEMMOVE_C)
DEFINE_EXTERNAL(void*,  memmove, void *, const void *, size_t);
#endif

//DEFINE_EXTERNAL(void*,  memset, void *, int, size_t);
#if defined(MEMMEM_C)
DEFINE_EXTERNAL(void*,  memmem, const void *, size_t, const void *, size_t) /*__purefunc*/;
#endif

#if defined(STRCHR_CPP)
DEFINE_EXTERNAL(char*,  strchr, const char *, int) /*__purefunc*/;
#endif

#if defined(STRRCHR_CPP)
DEFINE_EXTERNAL(char*,  strrchr, const char *, int) /*__purefunc*/;
#endif

#if defined(STRLEN_C)
DEFINE_EXTERNAL(size_t, strlen, const char *) /*__purefunc*/;
#endif

#if defined(STRCMP_C)
DEFINE_EXTERNAL(int,    strcmp, const char *, const char *) /*__purefunc*/;
#endif

#if defined(STPCPY_C)
DEFINE_EXTERNAL(char*,  stpcpy, char* __restrict, const char* __restrict);
#endif

#ifdef STRCPY_C
DEFINE_EXTERNAL(char*,  strcpy, char* __restrict, const char* __restrict);
#endif

#ifdef STRCAT_C
DEFINE_EXTERNAL(char*,  strcat, char* __restrict, const char* __restrict);
#endif

#ifdef STRCASECMP_C
DEFINE_EXTERNAL(int, strcasecmp, const char*, const char*) /*__purefunc*/;
DEFINE_EXTERNAL(int, strncasecmp, const char*, const char*, size_t) /*__purefunc*/;
#endif

#if defined(LOCALE_CPP)
DEFINE_EXTERNAL(int, strcasecmp_l, const char*, const char*, locale_t) /*__purefunc*/;
DEFINE_EXTERNAL(int, strncasecmp_l, const char*, const char*, size_t, locale_t) /*__purefunc*/;
DEFINE_EXTERNAL(char*, strerror_l, int, locale_t);
DEFINE_EXTERNAL(int,    strcoll_l, const char *, const char *, locale_t) /*__purefunc*/;
DEFINE_EXTERNAL(size_t, strxfrm_l, char* __restrict, const char* __restrict, size_t, locale_t);
#endif

#if defined(STRDUP_C)
DEFINE_EXTERNAL(char*,  strdup, const char *);
#endif

#if defined(STRSTR_C)
DEFINE_EXTERNAL(char*,  strstr, const char *, const char *) /*__purefunc*/;
#endif

#if defined(STRCASESTR_C)
DEFINE_EXTERNAL(char*,  strcasestr, const char *haystack, const char *needle) /*__purefunc*/;
#endif
#if defined(STRTOK_C)
DEFINE_EXTERNAL(char*,  strtok, char* __restrict, const char* __restrict);
DEFINE_EXTERNAL(char*,  strtok_r, char* __restrict, const char* __restrict, char** __restrict);
#endif

#if defined(STRERROR_CPP)
DEFINE_EXTERNAL(char*, strerror, int);
#endif
#if defined(STRERROR_R_CPP)
//DEFINE_EXTERNAL(char*, strerror_r, int, char*, size_t) __RENAME(__gnu_strerror_r);
DEFINE_EXTERNAL(int, strerror_r, int, char*, size_t);
#endif

#if defined(STRNLEN_C)
DEFINE_EXTERNAL(size_t, strnlen, const char *, size_t) /*__purefunc*/;
#endif
#if defined(STRNCAT_C)
DEFINE_EXTERNAL(char*,  strncat, char* __restrict, const char* __restrict, size_t);
#endif
#if defined(STRNDUP_C)
DEFINE_EXTERNAL(char*,  strndup, const char *, size_t);
#endif
#if defined(STRNCMP_C)
DEFINE_EXTERNAL(int,    strncmp, const char *, const char *, size_t) /*__purefunc*/;
#endif
#if defined(STPNCPY_C)
DEFINE_EXTERNAL(char*,  stpncpy, char* __restrict, const char* __restrict, size_t);
#endif
#if defined(STRNCPY_C)
DEFINE_EXTERNAL(char*,  strncpy, char* __restrict, const char* __restrict, size_t);
#endif
#if defined(STRLCAT_C)
DEFINE_EXTERNAL(size_t, strlcat, char* __restrict, const char* __restrict, size_t);
#endif
#if defined(STRLCPY_C)
DEFINE_EXTERNAL(size_t, strlcpy, char* __restrict, const char* __restrict, size_t);
#endif

#if defined(STRCSPN_C)
DEFINE_EXTERNAL(size_t, strcspn, const char *, const char *) /*__purefunc*/;
#endif
#if defined(STRPBRK_C)
DEFINE_EXTERNAL(char*,  strpbrk, const char *, const char *) /*__purefunc*/;
#endif
#if defined(STRSEP_C)
DEFINE_EXTERNAL(char*,  strsep, char** __restrict, const char* __restrict);
#endif
#if defined(STRSPN_C)
DEFINE_EXTERNAL(size_t, strspn, const char *, const char *);
#endif
#if defined(STRSIGNAL_CPP)
DEFINE_EXTERNAL(char*,  strsignal, int  sig);
#endif
#if defined(STRCOLL_C)
DEFINE_EXTERNAL(int,    strcoll, const char *, const char *) /*__purefunc*/;
#endif

#if defined(STRXFRM_C)
DEFINE_EXTERNAL(size_t, strxfrm, char* __restrict, const char* __restrict, size_t);
#endif
