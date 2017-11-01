/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#undef NDEBUG
#include <cassert>

int main() {
  // make sure that no lib is preloaded. This test case fails when a lib having
  // the symbol 'close' is in LD_PRELOAD. In that case, expected_addr will point
  // to the address in the LD_PRELOADed lib, while get_libc_close_ptr() will
  // point to the address in the original libc.so. In order to prevent such
  // case, this test case is executed in a standalone process where the environment
  // variable isn't inherited from the shell.
  //
  // LD_PRELOAD is being set in some partner devices to hook into some libc functions.
  assert(getenv("LD_PRELOAD") == nullptr);

  void* library_with_close = dlopen("libtest_check_rtld_next_from_library.so", RTLD_NOW);
  assert(library_with_close != nullptr);
  void* expected_addr = dlsym(RTLD_DEFAULT, "close");
  assert(expected_addr != nullptr);
  typedef void* (*get_libc_close_ptr_fn_t)();
  get_libc_close_ptr_fn_t get_libc_close_ptr =
      reinterpret_cast<get_libc_close_ptr_fn_t>(dlsym(library_with_close, "get_libc_close_ptr"));
  assert(get_libc_close_ptr != nullptr);
  assert(expected_addr == get_libc_close_ptr());

  dlclose(library_with_close);
  printf("success");
  return 0;
}
