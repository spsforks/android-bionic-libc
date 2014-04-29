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

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#include <string>

TEST(atexit, combined_test) {
  int control_atexit = 0;
  int control_staticdtors = 0;
  void* handle = dlopen("libtest_atexit.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);

  // register control pointers
  void* sym = dlsym(handle, "register_atexit");
  ASSERT_TRUE(sym != NULL);
  reinterpret_cast<void (*)(int*, int*)>(sym)(&control_atexit, &control_staticdtors);

  ASSERT_EQ(0, dlclose(handle));
  // this test verifies atexit call from atexit handler. as well as the order of calls
  ASSERT_EQ(8, control_atexit);

  // this test verifies correct parameter passing to static d-tors
  //
  // TODO: this test is incomplete, for example openbsd (others have the same problem) is
  // relying on *dso value as indicator of function type).
  // This leads to the bug when __cxa_finalize passes invalid 'this' to d-tor (usually null, but strictly speaking undefined).
  // this particular test is going to pass because fn_dso is not null for the library.
  ASSERT_EQ(1, control_staticdtors);
}
