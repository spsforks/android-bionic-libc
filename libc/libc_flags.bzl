# Copts for libc-related libraries.
LIBC_CFLAGS = [
    "-D_LIBC=1",
    "-D__BIONIC_LP32_USE_STAT64",
    "-Wextra",
    "-Wunused",
    "-Wno-char-subscripts",
    "-Wno-deprecated-declarations",
    "-Wno-gcc-compat",
    # TODO(b/147666318): Included in bionic's common libc flags, but causes some Bazel targets
    # to fail.
    # "-Wframe-larger-than=2048",
    # Try to catch typical 32-bit assumptions that break with 64-bit pointers.
    "-Werror=pointer-to-int-cast",
    "-Werror=int-to-pointer-cast",
    "-Werror=type-limits",
    "-Werror",
    # Clang's exit-time destructor registration hides __dso_handle, but
    # __dso_handle needs to have default visibility on ARM32. See b/73485611.
    "-Wexit-time-destructors",
    # GWP-ASan requires platform TLS.
    "-fno-emulated-tls",
    "-DUSE_SCUDO",
]
