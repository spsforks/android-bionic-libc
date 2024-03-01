TEST(elftls_dl, dtv_resize) {
#if defined(__BIONIC__)
#define LOAD_LIB(soname) ({                           \
    auto lib = dlopen(soname, RTLD_LOCAL | RTLD_NOW); \
    ASSERT_NE(nullptr, lib);                          \
    reinterpret_cast<int(*)()>(dlsym(lib, "bump"));   \
  })

  auto dtv = []() -> TlsDtv* { return __get_tcb_dtv(__get_bionic_tcb()); };

  static_assert(sizeof(TlsDtv) == 3 * sizeof(void*),
                "This test assumes that the Dtv has a 3-word header");

  // Initially there are 4 modules (5 w/ hwasan):
  //  - the main test executable
  //  - libc
  //  - libtest_elftls_shared_var
  //  - libtest_elftls_tprel
  //  - w/ hwasan: libclang_rt.hwasan

  // The initial DTV is an empty DTV with no generation and a size of 0.
  TlsDtv* zero_dtv = dtv();
  ASSERT_EQ(0u, zero_dtv->count);
  ASSERT_EQ(nullptr, zero_dtv->next);
  ASSERT_EQ(kTlsGenerationNone, zero_dtv->generation);

  // Load module 5 (6 w/ hwasan).
  auto func1 = LOAD_LIB("libtest_elftls_dynamic_filler_1.so");
  ASSERT_EQ(101, func1());

  // After loading one module, the DTV should be initialized to the next
  // power-of-2 size (including the header).
  TlsDtv* initial_dtv = dtv();
  ASSERT_EQ(running_with_hwasan() ? 13u : 5u, dtv()->count);
  ASSERT_EQ(zero_dtv, initial_dtv->next);
  ASSERT_LT(0u, initial_dtv->generation);

  // Load module 6 (7 w/ hwasan).
  auto func2 = LOAD_LIB("libtest_elftls_dynamic_filler_2.so");
  ASSERT_EQ(102, func1());

#if defined(__aarch64__)
  // The arm64 TLSDESC resolver doesn't update the DTV if it is new enough for
  // the given access.
  ASSERT_EQ(running_with_hwasan() ? 13u : 5u, dtv()->count);
#else
  // __tls_get_addr updates the DTV anytime the generation counter changes.
  ASSERT_EQ(13u, dtv()->count);
#endif

  ASSERT_EQ(201, func2());
  TlsDtv* new_dtv = dtv();
  if (!running_with_hwasan()) {
    ASSERT_NE(initial_dtv, new_dtv);
    ASSERT_EQ(initial_dtv, new_dtv->next);
  }
  ASSERT_EQ(13u, new_dtv->count);

  // Load module 7 (8 w/ hwasan).
  auto func3 = LOAD_LIB("libtest_elftls_dynamic_filler_3.so");
  ASSERT_EQ(103, func1());
  ASSERT_EQ(202, func2());
  ASSERT_EQ(301, func3());

  ASSERT_EQ(new_dtv, dtv());

#undef LOAD_LIB
#else
  GTEST_SKIP() << "test doesn't apply to glibc";
#endif
}
