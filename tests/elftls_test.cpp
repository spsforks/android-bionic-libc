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

#include <dlfcn.h>
#include <gtest/gtest.h>

#include <thread>

#include "gtest_globals.h"
#include "private/__get_tls.h"
#include "utils.h"

// Specify the LE access model explicitly. This file is compiled into the
// bionic-unit-tests executable, but the compiler sees an -fpic object file
// output into a static library, so it defaults to dynamic TLS accesses.

// This variable will be zero-initialized (.tbss)
__attribute__((tls_model("local-exec"))) static __thread int tlsvar_le_zero;

// This variable will have an initializer (.tdata)
__attribute__((tls_model("local-exec"))) static __thread int tlsvar_le_init = 10;

// Access libtest_elftls_shared_var.so's TLS variable using an IE access.
__attribute__((tls_model("initial-exec"))) extern "C" __thread int elftls_shared_var;

TEST(elftls, basic_le) {
  // Check the variables on the main thread.
  ASSERT_EQ(11, ++tlsvar_le_init);
  ASSERT_EQ(1, ++tlsvar_le_zero);

  // Check variables on a new thread.
  std::thread([] {
    ASSERT_EQ(11, ++tlsvar_le_init);
    ASSERT_EQ(1, ++tlsvar_le_zero);
  }).join();
}

TEST(elftls, shared_ie) {
  ASSERT_EQ(21, ++elftls_shared_var);
  std::thread([] {
    ASSERT_EQ(21, ++elftls_shared_var);
  }).join();
}

TEST(elftls, dlopen_static) {
  // libtest_elftls_static.so can be dlopen'ed, even though it contains a TLS IE
  // access, because its IE access references a TLS variable from
  // libtest_elftls_shared_var.so, which is DT_NEEDED by the executable. This
  // pattern appears prominently in sanitizers, which use TLS IE instrumentation
  // in shared objects to access special variables exported from the executable
  // or from a preloaded solib.
  void* lib = dlopen("libtest_elftls_static.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto missing_weak_tls_addr = reinterpret_cast<int*(*)()>(dlsym(lib, "missing_weak_tls_addr"));
  auto bump_shared_var = reinterpret_cast<int(*)()>(dlsym(lib, "bump_shared_var"));
  ASSERT_NE(nullptr, missing_weak_tls_addr);
  ASSERT_NE(nullptr, bump_shared_var);

  ASSERT_EQ(static_cast<void*>(__get_tls()), missing_weak_tls_addr());
  ASSERT_EQ(21, ++elftls_shared_var);
  ASSERT_EQ(22, bump_shared_var());

  std::thread([missing_weak_tls_addr, bump_shared_var] {
    ASSERT_EQ(static_cast<void*>(__get_tls()), missing_weak_tls_addr());
    ASSERT_EQ(21, ++elftls_shared_var);
    ASSERT_EQ(22, bump_shared_var());
  }).join();
}

TEST(elftls, dlopen_static_error) {
  std::string helper = GetTestlibRoot() +
      "/elftls_dlopen_static_helper/elftls_dlopen_static_helper";
  std::string src_path = GetTestlibRoot() + "/libtest_elftls_static.so";
  std::string dst_path = GetTestlibRoot() + "/libtest_elftls_shared_var.so";
#if defined(__BIONIC__)
  std::string error =
      "dlerror: dlopen failed: TLS symbol \"elftls_shared_var\" in dlopened \"" + dst_path + "\" " +
      "referenced from \"" + src_path + "\" using IE access model\n";
#else
  // glibc will reserve some surplus static TLS memory, allowing this test to pass.
  std::string error = "success\n";
#endif

  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, error.c_str());
}
