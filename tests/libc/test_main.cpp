/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <gtest/gtest.h>

extern "C" 
{
int clock_main(void);
int exec_deconstructor_main(void);
int addrinfo_main(void);
int gethostbyname_main(void);
int gethostname_main(void);
int pthread_cleanup_push_main(void);
int semaphore_main(void);
int sem_post_main(void);
int seteuid_main(void);
int mutex_main(void);
int cond_main(void);
int getgrouplist_main(void);
int netinet_main(void);
int vfprintf_leak_main(void);
int cpu_set_main(void);
int pthread_mutex_main(void);
int pthread_rwlock_main(void);
int relocs_main(void);
int sysconf_main(void);
int dlclose_destruction_main(void);
}

int static_cpp_mutex_main(void);
int static_init_main(void);

TEST(os, clock) {
  int ret = clock_main();
  ASSERT_EQ(ret, 0);
}


TEST(net, netinet) {
  int ret = netinet_main();
  ASSERT_EQ(ret, 0);
}
TEST(net, gethostname) {
  int ret = gethostname_main();
  ASSERT_EQ(ret, 0);
}

TEST(pthreadmore, cleanup_push) {
  int ret = pthread_cleanup_push_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, semaphore) {
  int ret = semaphore_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, sem_post) {
  int ret = sem_post_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, seteuid) {
  int ret = seteuid_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, mutex) {
  int ret = mutex_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, cond) {
  int ret = cond_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, getgrouplist) {
  int ret = getgrouplist_main();
  ASSERT_EQ(ret, 0);
}

TEST(pthreadmore, mutex) {
  int ret = pthread_mutex_main();
  ASSERT_EQ(ret, 0);
}

TEST(os, sysconf) {
  int ret = sysconf_main();
  ASSERT_EQ(ret, 0);
}


