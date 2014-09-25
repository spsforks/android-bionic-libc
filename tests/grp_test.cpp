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

#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <gtest/gtest.h>

TEST(grp, getgrouplist) {
  //assume there is the shell user,
  //and it has more than 0 groups
  const char *user = "shell";
  int numgroups = 0;

  struct passwd* pw = getpwnam(user);
  int ret = getgrouplist(user, pw->pw_gid, NULL, &numgroups);
  ASSERT_EQ(-1, ret);
  ASSERT_LT(0, numgroups);

  gid_t* groups = (gid_t*)malloc(numgroups*sizeof(gid_t));
  ASSERT_TRUE(NULL != groups);
  ret = getgrouplist(user, pw->pw_gid, groups, &numgroups);
  ASSERT_GE(ret, numgroups);

  bool found_pw_group = false;
  int got_group_count = 0;
  for (int j = 0; j < numgroups; j++) {
    if (! groups[j]){
        continue;
    }
    if ((int)(pw->pw_gid) == (int)(groups[j])){
        found_pw_group = true;
    }
    got_group_count++;
  }

  ASSERT_TRUE(found_pw_group);
  ASSERT_EQ(got_group_count, numgroups);

  free(groups);
}
