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
DEFINE_EXTERNAL(int, pthread_mutexattr_settype, pthread_mutexattr_t *attr, int type));
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

#ifdef MALLOC_DEBUG_COMMON
DEFINE_EXTERNAL(void*, calloc, size_t n_elements, size_t elem_size);
DEFINE_EXTERNAL(void, free, void* mem);
DEFINE_EXTERNAL(struct mallinfo, mallinfo);
DEFINE_EXTERNAL(void*, malloc, size_t bytes);
DEFINE_EXTERNAL(size_t, malloc_usable_size, const void* mem);
DEFINE_EXTERNAL(void*, memalign, size_t alignment, size_t bytes);
DEFINE_EXTERNAL(int, posix_memalign, void** memptr, size_t alignment, size_t size);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
DEFINE_EXTERNAL(void*, pvalloc, size_t bytes);
#endif
DEFINE_EXTERNAL(void*, realloc, void* oldMem, size_t bytes);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
DEFINE_EXTERNAL(void*, valloc, size_t bytes);
#endif
#endif

