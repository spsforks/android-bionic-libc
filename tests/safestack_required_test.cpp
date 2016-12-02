#include <gtest/gtest.h>
#include "utils.h"
#include <libgen.h>

#if defined(__BIONIC__)
extern "C" void** __safestack_pointer_address();
extern const char* __progname;
#endif

// Test SafeStack-required ELF note.
// helper_suffix is "A_B" where A describes the main executable build, and B - the library build:
// no safestack (no), safestack (yes), or safestack-required (req).
// expect_success is false when this combination is expected to fail in the initial link.
// The helper executable (if expect_success == true) tests dlopen() behavior.
static void test_safestack_required(const char* helper_suffix, bool expect_success) {
#if defined(__BIONIC__)
  bool safestack_enabled = *__safestack_pointer_address() != nullptr;
  if (!safestack_enabled) {
    return;
  }

  char* progname = strdup(__progname);
  char* here = dirname(progname);
  std::string helper = std::string(here) + "/../bionic-loader-test-libs/test_safestack_" +
                       helper_suffix + "/test_safestack_" + helper_suffix;

  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  if (expect_success) {
    eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0,
            "safestack test done\n");
  } else {
    eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, -SIGABRT, nullptr);
  }
#else
  (void)helper_suffix;
  (void)expect_success;
#endif
}

TEST(safestack_required, no_no) {
  test_safestack_required("no_no", true);
}

TEST(safestack_required, no_yes) {
  test_safestack_required("no_no", true);
}

TEST(safestack_required, yes_yes) {
  test_safestack_required("no_no", true);
}

TEST(safestack_required, no_req) {
  test_safestack_required("no_req", false);
}

TEST(safestack_required, yes_req) {
  test_safestack_required("yes_req", true);
}

TEST(safestack_required, req_no) {
  test_safestack_required("req_no", false);
}

TEST(safestack_required, req_yes) {
  test_safestack_required("req_yes", true);
}
