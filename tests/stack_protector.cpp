/*
 * Copyright (C) 2012 The Android Open Source Project
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
/*
 * Contributed by: Intel Corporation
 */

/*
 * Stack protector test. x86 only.
 * Checks that TLS stack canary (%gs:0x14) is different per each thread.
*/

#include <gtest/gtest.h>

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

unsigned int get_guard() {
  unsigned int i;
  asm ("mov %%gs:0x14, %0": "=d" (i));
  return i;
}


void* th_test(void* q) {
  static int prevguard;
  static pthread_t prevtid;

  unsigned int guard;
  pthread_t tid;

  tid = gettid();
  guard = get_guard();

  // tid is the same
  if (prevtid == tid) {
      return (void *)1;
  }
  // guard is the same
  if (guard == prevguard || guard == 0) {
      return (void *)2;
  }
  printf("[%d] gs:0x14 = %#x\n", (int)tid , guard);
  prevtid = tid;
  prevguard = guard;
  return (void *)8;
}


TEST(stack_protector, guard_per_thread) {
  int i = 0;
  void *pr;

  while (i < 10) {
    pthread_t thread_id;
    ASSERT_EQ(0, pthread_create(&thread_id, NULL, th_test, NULL));
    ASSERT_EQ(0, pthread_join(thread_id, &pr));
    ASSERT_EQ(8, (int)pr);
    ++i;
  }
}

