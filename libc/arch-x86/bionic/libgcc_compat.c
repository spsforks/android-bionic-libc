extern char __popcount_tab;
extern char __popcountsi2;

void* __bionic_libgcc_compat_symbols[] = {
    &__popcount_tab,
    &__popcountsi2
};
