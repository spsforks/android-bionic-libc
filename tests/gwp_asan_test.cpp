#include <gtest/gtest.h>
#include <cstdio>
#include <string>

#if defined(__BIONIC__)

#include "gwp_asan/options.h"
#include "platform/bionic/malloc.h"
#include "utils.h"

void RunGwpAsanTest(const char* test_name) {
  ExecTestHelper eh;
  eh.SetEnv({"GWP_ASAN_SAMPLE_RATE=1", "GWP_ASAN_PROCESS_SAMPLING=1", "GWP_ASAN_MAX_ALLOCS=40000",
             nullptr});
  std::string filter_arg = "--gtest_filter=";
  filter_arg += test_name;
  std::string exec(testing::internal::GetArgvs()[0]);
  eh.SetArgs({exec.c_str(), "--gtest_also_run_disabled_tests", filter_arg.c_str()});
  eh.Run([&]() { execve(exec.c_str(), eh.GetArgs(), eh.GetEnv()); },
         /* expected_exit_status */ 0,
         // |expected_output_regex|, ensure at least one test ran:
         R"(\[  PASSED  \] [1-9]+0? test)");
}

// This file implements "torture testing" under GWP-ASan, where we sample every
// single allocation. The upper limit for the number of GWP-ASan allocations in
// the torture mode is is generally 40,000, so that svelte devices don't
// explode, as this uses ~163MiB RAM (4KiB per live allocation).
TEST(gwp_asan_integration, malloc_tests_under_torture) {
  RunGwpAsanTest("malloc.*:-malloc.mallinfo*");
}

#endif  // defined(__BIONIC__)
