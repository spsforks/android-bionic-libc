/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <limits.h>

#include <gtest/gtest.h>

#include "MallocDebugConfig.h"

#include "log_fake.h"

extern "C" int property_set(const char*, const char*);

class MallocDebugConfigTest : public ::testing::Test {
 protected:
  void SetUp() override {
    resetLogs();
  }

  void TearDown() override {
  }
};

std::string usage_string(
  "6 malloc_debug malloc debug options usage:\n"
  "6 malloc_debug \n"
  "6 malloc_debug   front_guard[=XX]\n"
  "6 malloc_debug     Enables a front guard on all allocations. If XX is set\n"
  "6 malloc_debug     it sets the number of bytes in the guard. The default is\n"
  "6 malloc_debug     32 bytes.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   rear_guard[=XX]\n"
  "6 malloc_debug     Enables a rear guard on all allocations. If XX is set\n"
  "6 malloc_debug     it sets the number of bytes in the guard. The default is\n"
  "6 malloc_debug     32 bytes.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   guard[=XX]\n"
  "6 malloc_debug     Enables both a front guard and a rear guard on all allocations.\n"
  "6 malloc_debug     If XX is set it sets the number of bytes in both guards.\n"
  "6 malloc_debug     The default is 32 bytes.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   backtrace[=XX]\n"
  "6 malloc_debug     Enable capturing the backtrace at the point of allocation.\n"
  "6 malloc_debug     If XX is set it sets the number of backtrace frames.\n"
  "6 malloc_debug     The default is 16 frames.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   enable_bt_on_signal[=XX]\n"
  "6 malloc_debug     Enable capturing the backtrace at the point of allocation.\n"
  "6 malloc_debug     The backtrace capture is not enabled until the process\n"
  "6 malloc_debug     receives SIGRTMIN + 10. If XX is set it sets the number\n"
  "6 malloc_debug     of backtrace frames. The default is 16 frames.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill_on_alloc[=XX]\n"
  "6 malloc_debug     On first allocation, fill with the value 0xeb.\n"
  "6 malloc_debug     If XX is set it will only fill up to XX bytes of the\n"
  "6 malloc_debug     allocation. The default is to fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill_on_free[=XX]\n"
  "6 malloc_debug     On free, fill with the value 0xef. If XX is set it will\n"
  "6 malloc_debug     only fill up to XX bytes of the allocation. The default is to\n"
  "6 malloc_debug     fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   fill[=XX]\n"
  "6 malloc_debug     On both first allocation free, fill with the value 0xeb on\n"
  "6 malloc_debug     first allocation and the value 0xef. If XX is set, only fill\n"
  "6 malloc_debug     up to XX bytes. The default is to fill the entire allocation.\n"
  "6 malloc_debug \n"
  "6 malloc_debug   expand_alloc[=XX]\n"
  "6 malloc_debug     Allocate an extra number of bytes for every allocation call.\n"
  "6 malloc_debug     If XX is set, that is the number of bytes to expand the\n"
  "6 malloc_debug     allocation by. The default is 16 bytes.\n"
);

TEST_F(MallocDebugConfigTest, unknown_option) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "unknown_option");
  ASSERT_FALSE(config.SetFromProperties());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: unknown option unknown_option\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, invalid_number) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace=XXX");
  ASSERT_FALSE(config.SetFromProperties());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg("6 malloc_debug malloc_testing: bad value for option 'backtrace'\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, no_space) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace=10front_guard");
  ASSERT_FALSE(config.SetFromProperties());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace', "
      "non space found after option: front_guard\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, overflow) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace=999999999999999999");
  ASSERT_FALSE(config.SetFromProperties());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: bad value for option 'backtrace': "
      "Math result not representable\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}


TEST_F(MallocDebugConfigTest, set_value_error) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "enable_record=12");
  ASSERT_FALSE(config.SetFromProperties());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string log_msg(
      "6 malloc_debug malloc_testing: value set for option 'enable_record' "
      "which does not take a value\n");
  ASSERT_STREQ((log_msg + usage_string).c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, space_before_equal) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace  =10");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(10, config.backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, space_after_equal) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace=  10");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(10, config.backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, extra_space) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "   backtrace=64   ");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(64, config.backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, multiple_options) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "  backtrace=64   front_guard=24");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE | FRONT_GUARD, config.options);
  ASSERT_EQ(64, config.backtrace_frames);
  ASSERT_EQ(24, config.front_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, front_guard) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "front_guard=24");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FRONT_GUARD, config.options);
  ASSERT_EQ(24, config.front_guard_bytes);

  property_set("libc.debug.malloc.options", "front_guard");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FRONT_GUARD, config.options);
  ASSERT_EQ(32, config.front_guard_bytes);

  property_set("libc.debug.malloc.options", "front_guard=39");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FRONT_GUARD, config.options);
  ASSERT_EQ(40, config.front_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, rear_guard) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "rear_guard=50");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(REAR_GUARD, config.options);
  ASSERT_EQ(50, config.rear_guard_bytes);

  property_set("libc.debug.malloc.options", "rear_guard");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(REAR_GUARD, config.options);
  ASSERT_EQ(32, config.rear_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, guard) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "guard=32");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD, config.options);
  ASSERT_EQ(32, config.front_guard_bytes);
  ASSERT_EQ(32, config.rear_guard_bytes);

  property_set("libc.debug.malloc.options", "guard");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FRONT_GUARD | REAR_GUARD, config.options);
  ASSERT_EQ(32, config.front_guard_bytes);
  ASSERT_EQ(32, config.rear_guard_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, backtrace) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "backtrace=64");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(64, config.backtrace_frames);

  property_set("libc.debug.malloc.options", "backtrace");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(16, config.backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, enable_bt_on_signal) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "enable_bt_on_signal=64");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(64, config.backtrace_frames);

  property_set("libc.debug.malloc.options", "enable_bt_on_signal");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(BACKTRACE, config.options);
  ASSERT_EQ(16, config.backtrace_frames);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_alloc) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "fill_on_alloc=64");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_ALLOC, config.options);
  ASSERT_EQ(64, config.fill_on_alloc_bytes);

  property_set("libc.debug.malloc.options", "fill_on_alloc");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_ALLOC, config.options);
  ASSERT_EQ(LONG_MAX, config.fill_on_alloc_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill_on_free) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "fill_on_free=64");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_FREE, config.options);
  ASSERT_EQ(64, config.fill_on_free_bytes);

  property_set("libc.debug.malloc.options", "fill_on_free");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_FREE, config.options);
  ASSERT_EQ(LONG_MAX, config.fill_on_free_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, fill) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "fill=64");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config.options);
  ASSERT_EQ(64, config.fill_on_alloc_bytes);
  ASSERT_EQ(64, config.fill_on_free_bytes);

  property_set("libc.debug.malloc.options", "fill");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(FILL_ON_ALLOC | FILL_ON_FREE, config.options);
  ASSERT_EQ(LONG_MAX, config.fill_on_alloc_bytes);
  ASSERT_EQ(LONG_MAX, config.fill_on_free_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, expand_alloc) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "expand_alloc=1234");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(EXPAND_ALLOC, config.options);
  ASSERT_EQ(1234, config.expand_alloc_bytes);

  property_set("libc.debug.malloc.options", "expand_alloc");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(EXPAND_ALLOC, config.options);
  ASSERT_EQ(16, config.expand_alloc_bytes);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugConfigTest, enable_record) {
  MallocDebugConfig config;

  property_set("libc.debug.malloc.options", "enable_record");
  ASSERT_TRUE(config.SetFromProperties());
  ASSERT_EQ(RECORD_ALLOC, config.options);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}
