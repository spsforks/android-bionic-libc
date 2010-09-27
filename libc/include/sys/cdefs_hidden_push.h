/* Ignore GCC visibility if debug build */
#if !defined(DEBUG) || defined(NDEBUG)

#if defined(__LIBC_HIDDEN_PUSH_DEFINED)
#error "Disallow nested GCC visibility declaration"
#endif

#define __LIBC_HIDDEN_PUSH_DEFINED
#undef __LIBC_HIDDEN_POP_DEFINED

#pragma GCC visibility push(hidden)

#endif
