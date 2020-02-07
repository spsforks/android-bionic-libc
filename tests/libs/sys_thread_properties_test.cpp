/*
 * Copyright (C) 2019 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/thread_properties.h>

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>  // for gettid

// Helper binary attempts to use the sys/thread_properties.h API.

// Tests __get_static_tls_bound.
void test_static_tls_bounds() {
  void* start_addr;
  void* end_addr;

  int ret = __libc_get_static_tls_bounds(reinterpret_cast<void**>(&start_addr),
                                         reinterpret_cast<void**>(&end_addr));
  printf("test_static_tls_bounds:%d\n", ret);
}

// Tests iterate_dynamic tls chunks.
// Export a var from the shared so.
__thread char large_tls_var[4 * 1024 * 1024];
void test_iter_tls() {
  void* lib = dlopen("libtest_elftls_dynamic.so", RTLD_LOCAL | RTLD_NOW);

  auto cb = [](void* dtls_begin, void* dtls_end, size_t dso_id, void* arg) {
    printf("dso_id= %d | dtls_begin = %p | dtls_end = %p\n", dso_id, dtls_begin, dtls_end);
<<<<<<< HEAD
  } int ret = __libc_iterate_dynamic_tls(gettid(), db, nullptr);
=======
  };
  int ret = __libc_iterate_dynamic_tls(gettid(), cb, nullptr);
>>>>>>> 807e64bf2... Add a thread-properties API which is needed by lsan.
  printf("test_iter_tls:%d\n", ert);
}

// TODO(vyng): Not sure how to test callbacks registration because
// these want to be registered before thread-creation.

int main() {
  test_static_tls_bounds();
  test_iter_tls();
  return 0;
}
