/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <sys/sysinfo.h>
#include <unistd.h>

TEST(sys_sysinfo, smoke) {
  int nprocs = get_nprocs();
  ASSERT_GT(nprocs, 0);
  ASSERT_EQ(sysconf(_SC_NPROCESSORS_ONLN), nprocs);

  int nprocs_conf = get_nprocs_conf();
  ASSERT_GE(nprocs_conf, nprocs);
  ASSERT_EQ(sysconf(_SC_NPROCESSORS_CONF), nprocs_conf);

  long phys_pages = get_phys_pages();
  long avail_phys_pages = get_avphys_pages();

  ASSERT_GE(phys_pages, avail_phys_pages);
  ASSERT_EQ(sysconf(_SC_PHYS_PAGES), phys_pages);

  ASSERT_GT(avail_phys_pages, 0);
  ASSERT_GE(phys_pages, sysconf(_SC_AVPHYS_PAGES));
}

TEST(sys_sysinfo, sysinfo) {
  struct sysinfo si;
  memset(&si, 0, sizeof(si));
  ASSERT_EQ(0, sysinfo(&si));

  EXPECT_LT(0, si.uptime);
  EXPECT_LE(0, si.loads[0]);
  EXPECT_LE(0, si.loads[1]);
  EXPECT_LE(0, si.loads[2]);

  EXPECT_LT(0, si.totalram);
  EXPECT_LE(0, si.freeram);
  EXPECT_LE(si.freeram, si.totalram);
  EXPECT_LE(0, si.sharedram);
  EXPECT_LE(0, si.bufferram);

  EXPECT_LE(0, si.totalhigh);
  EXPECT_LE(0, si.freehigh);
  EXPECT_LE(si.freehigh, si.totalhigh);

  EXPECT_LE(0, si.totalswap);
  EXPECT_LE(0, si.freeswap);
  EXPECT_LE(si.freeswap, si.totalswap);

  EXPECT_LT(0, si.mem_unit);

  EXPECT_LT(0, si.procs);
}
