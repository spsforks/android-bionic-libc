/* Ignore GCC visibility if debug build */
#if !defined(DEBUG) || defined(NDEBUG)

#if !defined(__LIBC_HIDDEN_PUSH_DEFINED)
#error "Unbalanced pair of GCC visibility push/pop"
#endif

#define __LIBC_HIDDEN_POP_DEFINED
#undef __LIBC_HIDDEN_PUSH_DEFINED

#pragma GCC visibility pop

#endif
