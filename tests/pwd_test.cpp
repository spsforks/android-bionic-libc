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

#include <pwd.h>

#include <gtest/gtest.h>
#include "CrashCatch.h"

const static std::string USER = "root";
const static int USER_UID = 0;

TEST(pwd, getpwnam)
{
    CRASH_CATCH();

    struct passwd *pw;

    { SCOPED_TRACE("getpwnam()");
       pw = getpwnam(USER.c_str());
       EXPECT_TRUE(pw);
       EXPECT_EQ(0, strcmp(USER.c_str(), pw->pw_name));
       EXPECT_FALSE(pw->pw_passwd);
       EXPECT_EQ(USER_UID, pw->pw_uid);
       EXPECT_FALSE(pw->pw_gid);
       EXPECT_EQ(0, strcmp("/", pw->pw_dir));
       EXPECT_EQ(0, strcmp("/system/bin/sh", pw->pw_shell));
    }
}

TEST(pwd, getpwuid)
{
    CRASH_CATCH();

    struct passwd *pw;

    { SCOPED_TRACE("getpwnam()");
       pw = getpwuid(USER_UID);
       EXPECT_TRUE(pw);
       EXPECT_EQ(0, strcmp(USER.c_str(), pw->pw_name));
       EXPECT_FALSE(pw->pw_passwd);
       EXPECT_EQ(USER_UID, pw->pw_uid);
       EXPECT_FALSE(pw->pw_gid);
       EXPECT_EQ(0, strcmp("/", pw->pw_dir));
       EXPECT_EQ(0, strcmp("/system/bin/sh", pw->pw_shell));
    }
}
