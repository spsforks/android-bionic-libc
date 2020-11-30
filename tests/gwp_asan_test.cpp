#include <gtest/gtest.h>
#include <cstdio>
#include <string>

#if defined(__BIONIC__)

#include "gwp_asan/options.h"
#include "platform/bionic/malloc.h"
#include "utils.h"

void RunGwpAsanTest(const char* test_name, const char* gwp_asan_options) {
  ExecTestHelper eh;
  std::string env_arg = "GWP_ASAN_OPTIONS=";
  env_arg += gwp_asan_options;
  eh.SetEnv({env_arg.c_str(), nullptr});
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

using Options = gwp_asan::options::Options;
using SampleRate_t = typeof(Options::SampleRate);
using SimultaneousAllocations_t = typeof(Options::MaxSimultaneousAllocations);
constexpr SampleRate_t kMockSampleRate = 1000;
constexpr unsigned kMockProcSampleRate = 2000;
constexpr SimultaneousAllocations_t kMockSimultaneousAllocs = 3000;

extern "C" bool ParseGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                                    char* options_str);

TEST(gwp_asan_integration, malloc_tests_under_torture) {
  RunGwpAsanTest("malloc.*:-malloc.mallinfo*", "1:1:40000");
}

void TestOptionsParsingFailed(char* options_str) {
  Options out;
  unsigned ps_out;

  EXPECT_FALSE(ParseGwpAsanOptions(&out, &ps_out, options_str));
}

void TestOptionsParsing(char* options_str, SampleRate_t expected_sample_rate,
                        unsigned expected_proc_sample_rate,
                        SimultaneousAllocations_t expected_simultaneous_allocs) {
  Options options_out;
  options_out.SampleRate = kMockSampleRate;
  options_out.MaxSimultaneousAllocations = kMockSimultaneousAllocs;
  unsigned process_sampling_out = kMockProcSampleRate;
  EXPECT_TRUE(ParseGwpAsanOptions(&options_out, &process_sampling_out, options_str));
  EXPECT_EQ(expected_sample_rate, options_out.SampleRate);
  EXPECT_EQ(expected_proc_sample_rate, process_sampling_out);
  EXPECT_EQ(expected_simultaneous_allocs, options_out.MaxSimultaneousAllocations);
}

// A short macro that ensures that any failures are easily traceable to the test
// line number, not the helper function line number.
#define RUN_TEST(invocation, arg...) \
  {                                  \
    SCOPED_TRACE("");                \
    invocation(arg);                 \
  }

// Unit tests for the GWP-ASan wrappers.
TEST(gwp_asan_unit, BadStrings) {
  char str_buf[255];
  strcpy(str_buf, "1:2:3:4");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1:2:3:");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, ":1:2:3");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, " 1:2:3");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1:2:3 ");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1 :2");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1: 2");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1 2");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "1 ");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, " 1");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, ":::");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "abcdef");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
  strcpy(str_buf, "a:b:c");
  RUN_TEST(TestOptionsParsingFailed, str_buf);
}

TEST(gwp_asan_unit, FullStrings) {
  char str_buf[255];
  strcpy(str_buf, "1:2:3");
  RUN_TEST(TestOptionsParsing, str_buf, 1, 2, 3);
  strcpy(str_buf, "1000:2000:3000");
  RUN_TEST(TestOptionsParsing, str_buf, 1000, 2000, 3000);
}

TEST(gwp_asan_unit, DefaultSampleRate) {
  char str_buf[255];
  strcpy(str_buf, ":2:3");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, 2, 3);
  strcpy(str_buf, "::3000");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, kMockProcSampleRate, 3000);
  strcpy(str_buf, ":3000:");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, 3000, kMockSimultaneousAllocs);
  strcpy(str_buf, "::");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, kMockProcSampleRate,
           kMockSimultaneousAllocs);
  strcpy(str_buf, ":");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, kMockProcSampleRate,
           kMockSimultaneousAllocs);
  strcpy(str_buf, "");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, kMockProcSampleRate,
           kMockSimultaneousAllocs);
}

TEST(gwp_asan_unit, DefaultPSRate) {
  char str_buf[255];
  strcpy(str_buf, "1::3");
  RUN_TEST(TestOptionsParsing, str_buf, 1, kMockProcSampleRate, 3);
  strcpy(str_buf, "::3000");
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate, kMockProcSampleRate, 3000);
}

TEST(gwp_asan_unit, InferredMaxAllocs) {
  char str_buf[255];
  sprintf(str_buf, "%d:2:", kMockSampleRate / 2);
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate / 2, 2, 2 * kMockSimultaneousAllocs);
  sprintf(str_buf, "%d::", kMockSampleRate * 2);
  RUN_TEST(TestOptionsParsing, str_buf, kMockSampleRate * 2, kMockProcSampleRate,
           kMockSimultaneousAllocs / 2);
}

#endif  // defined(__BIONIC__)
