/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <unistd.h>

#include <gtest/gtest.h>

#include "TemporaryFile.h"

#if defined(__BIONIC__)
#include "../libc/bionic/grp_pwd_file.cpp"

class FileUnmapper {
 public:
  FileUnmapper(MmapFileBase& file) : file_(file) {
  }
  ~FileUnmapper() {
    char* start;
    const char* end;
    if (!file_.GetFile(&start, &end)) {
      return;
    }
    size_t size = end - start + 1;
    munmap(start, size);
  }

 private:
  MmapFileBase& file_;
};

void FindAndCheckPasswdEntry(PasswdFile* file, const char* name, uid_t uid, gid_t gid,
                             const char* dir, const char* shell) {
  PasswdLine name_lines;
  ASSERT_TRUE(FindByName(file, name, &name_lines)) << name;

  passwd_state_t name_passwd_state;
  ASSERT_TRUE(name_lines.ToPasswd(&name_passwd_state));

  passwd& name_passwd = name_passwd_state.passwd_;
  EXPECT_STREQ(name, name_passwd.pw_name);
  EXPECT_EQ(nullptr, name_passwd.pw_passwd);
  EXPECT_EQ(uid, name_passwd.pw_uid);
  EXPECT_EQ(gid, name_passwd.pw_gid);
  EXPECT_EQ(nullptr, name_passwd.pw_gecos);
  EXPECT_STREQ(dir, name_passwd.pw_dir);
  EXPECT_STREQ(shell, name_passwd.pw_shell);

  PasswdLine id_line;
  ASSERT_TRUE(FindById(file, 1, &id_line)) << uid;

  passwd_state_t id_passwd_state;
  ASSERT_TRUE(name_lines.ToPasswd(&id_passwd_state));

  passwd& id_passwd = id_passwd_state.passwd_;
  EXPECT_STREQ(name, id_passwd.pw_name);
  EXPECT_EQ(nullptr, id_passwd.pw_passwd);
  EXPECT_EQ(uid, id_passwd.pw_uid);
  EXPECT_EQ(gid, id_passwd.pw_gid);
  EXPECT_EQ(nullptr, id_passwd.pw_gecos);
  EXPECT_STREQ(dir, id_passwd.pw_dir);
  EXPECT_STREQ(shell, id_passwd.pw_shell);
}

void FindAndCheckGroupEntry(GroupFile* file, const char* name, gid_t gid) {
  GroupLine name_lines;
  ASSERT_TRUE(FindByName(file, name, &name_lines)) << name;

  group_state_t name_group_state;
  ASSERT_TRUE(name_lines.ToGroup(&name_group_state));

  group& name_group = name_group_state.group_;
  EXPECT_STREQ(name, name_group.gr_name);
  EXPECT_EQ(nullptr, name_group.gr_passwd);
  EXPECT_EQ(gid, name_group.gr_gid);
  EXPECT_EQ(name_group.gr_name, name_group.gr_mem[0]);
  EXPECT_EQ(nullptr, name_group.gr_mem[1]);

  GroupLine id_line;
  ASSERT_TRUE(FindById(file, gid, &id_line)) << gid;

  group_state_t id_group_state;
  ASSERT_TRUE(id_line.ToGroup(&id_group_state));

  group& id_group = id_group_state.group_;
  EXPECT_STREQ(name, id_group.gr_name);
  EXPECT_EQ(nullptr, id_group.gr_passwd);
  EXPECT_EQ(gid, id_group.gr_gid);
  EXPECT_EQ(id_group.gr_name, id_group.gr_mem[0]);
  EXPECT_EQ(nullptr, id_group.gr_mem[1]);
}
#endif  // __BIONIC__

TEST(grp_pwd_file, passwd_file_one_entry) {
#if defined(__BIONIC__)
  TemporaryFile file;
  ASSERT_NE(-1, file.fd);
  static const char test_string[] = "name:password:1:2:user_info:dir:shell\n";
  write(file.fd, test_string, sizeof(test_string) - 1);

  PasswdFile passwd_file(file.filename);
  FileUnmapper unmapper(passwd_file);

  FindAndCheckPasswdEntry(&passwd_file, "name", 1, 2, "dir", "shell");

#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}

TEST(grp_pwd_file, group_file_one_entry) {
#if defined(__BIONIC__)
  TemporaryFile file;
  ASSERT_NE(-1, file.fd);
  static const char test_string[] = "name:password:1:one,two,three\n";
  write(file.fd, test_string, sizeof(test_string) - 1);

  GroupFile group_file(file.filename);
  FileUnmapper unmapper(group_file);

  FindAndCheckGroupEntry(&group_file, "name", 1);

#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}

TEST(grp_pwd_file, passwd_file_many_entries) {
#if defined(__BIONIC__)
  TemporaryFile file;
  ASSERT_NE(-1, file.fd);
  static const char test_string[] =
      "first:x:1:2::dir:shell\n"
      "abc1::3:4::def:abc\n"
      "abc2::5:4:abc::abc\n"
      "abc3::7:4:abc:def:\n"
      "abc4::9:4:::abc\n"
      "abc5::11:4:abc:def:abc\n"
      "middle-ish::13:4::/:/system/bin/sh\n"
      "abc7::15:4:abc::\n"
      "abc8::17:4:::\n"
      "abc9::19:4:abc:def:abc\n"
      "abc10::21:4:abc:def:abc\n"
      "abc11::23:4:abc:def:abc\n"
      "abc12::25:4:abc:def:abc\n"
      "abc13::27:4:abc:def:abc\n"
      "last::29:4::last_user_dir:last_user_shell\n";

  write(file.fd, test_string, sizeof(test_string) - 1);

  PasswdFile passwd_file(file.filename);
  FileUnmapper unmapper(passwd_file);

  FindAndCheckPasswdEntry(&passwd_file, "first", 1, 2, "dir", "shell");
  FindAndCheckPasswdEntry(&passwd_file, "middle-ish", 13, 4, "/", "/system/bin/sh");
  FindAndCheckPasswdEntry(&passwd_file, "last", 29, 4, "last_user_dir", "last_user_shell");

#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}

TEST(grp_pwd_file, group_file_many_entries) {
#if defined(__BIONIC__)
  TemporaryFile file;
  ASSERT_NE(-1, file.fd);
  static const char test_string[] =
      "first:password:1:one,two,three\n"
      "abc:def:2:group1,group2,group3\n"
      "abc:def:3:\n"
      "abc:def:4:\n"
      "abc:def:5:\n"
      "middle-ish:def_a_password_that_is_over_32_characters_long:6:\n"
      "abc:def:7:\n"
      "abc:def:8:\n"
      "abc:def:20:\n"
      "abc:def:25:\n"
      "abc:def:27:\n"
      "abc:def:52:\n"
      "last::800:\n";

  write(file.fd, test_string, sizeof(test_string) - 1);

  GroupFile group_file(file.filename);
  FileUnmapper unmapper(group_file);

  FindAndCheckGroupEntry(&group_file, "first", 1);
  FindAndCheckGroupEntry(&group_file, "middle-ish", 6);
  FindAndCheckGroupEntry(&group_file, "last", 800);

#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}
