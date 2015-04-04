/* STOPSHIP: remove this once the flounder blobs have been rebuilt (http://b/20065774). */

#if !defined(__clang__)

extern void __clear_cache(char*, char*) __attribute__((visibility("protected")));
extern char _Unwind_Backtrace __attribute__((visibility("protected")));
extern char _Unwind_GetIP __attribute__((visibility("protected")));

void* __bionic_libgcc_compat_symbols[] = {
    &__clear_cache,
    &_Unwind_Backtrace,
    &_Unwind_GetIP,
};

#endif
