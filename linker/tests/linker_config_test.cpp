/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "../linker_config.h"

#include <unistd.h>

#include <android-base/file.h>
#include <android-base/test_utils.h>


static const char* config_str =
  "# comment \n"
  "dir.test = /some/test/dir\n"
  "\n"
  "[test]\n"
  "\n"
  "enable.target.sdk.version = true\n"
  "additional.namespaces=system\n"
  "namespace.default.isolated = true\n"
  "namespace.default.search.paths = /vendor/${LIB}\n"
  "namespace.default.permitted.paths = /vendor/${LIB}\n"
  "namespace.default.asan.search.paths = /data:/vendor/${LIB}\n"
  "namespace.default.asan.permitted.paths = /data:/vendor\n"
  "namespace.default.link.system.shared_libs = libc.so:libm.so:libdl.so:libstdc++.so\n"
  "namespace.system.isolated = true\n"
  "namespace.system.search.paths = /system/${LIB}:/system/${LIB}/ndk\n"
  "namespace.system.permitted.paths = /system/${LIB}\n"
  "namespace.system.asan.search.paths = /data:/system/${LIB}\n"
  "namespace.system.asan.permitted.paths = /data:/system\n"
  "\n";

TEST(linker_config, smoke) {
#if defined(__LP64__)
  static const std::vector<std::string> kExpectedDefaultSearchPath =
        { "/vendor/lib64" };
  static const std::vector<std::string> kExpectedDefaultPermittedPath =
        { "/vendor/lib64" };
  static const std::vector<std::string> kExpectedSystemSearchPath =
        { "/system/lib64", "/system/lib64/ndk" };
  static const std::vector<std::string> kExpectedSystemPermittedPath =
        { "/system/lib64" };
#else
  static const std::vector<std::string> kExpectedDefaultSearchPath =
        { "/vendor/lib" };
  static const std::vector<std::string> kExpectedDefaultPermittedPath =
        { "/vendor/lib" };
  static const std::vector<std::string> kExpectedSystemSearchPath =
        { "/system/lib", "/system/lib/ndk" };
  static const std::vector<std::string> kExpectedSystemPermittedPath =
        { "/system/lib" };
#endif

  TemporaryFile tmp_file;
  close(tmp_file.fd);
  tmp_file.fd = -1;

  android::base::WriteStringToFile(config_str, tmp_file.path);

  // read config
  const BinaryConfig* config = nullptr;
  std::string error_msg;
  ASSERT_TRUE(BinaryConfig::read_binary_config(tmp_file.path,
                                               "/some/test/dir/113/some-binary",
                                               false,
                                               &config,
                                               &error_msg)) << error_msg;
  ASSERT_TRUE(config != nullptr);
  ASSERT_TRUE(error_msg.empty());

  ASSERT_EQ(113U, config->target_sdk_version());

  const NamespaceConfig* default_ns_config = config->default_namespace_config();
  ASSERT_TRUE(default_ns_config != nullptr);

  ASSERT_TRUE(default_ns_config->isolated());
  ASSERT_EQ(kExpectedDefaultPermittedPath, default_ns_config->search_paths());
  ASSERT_EQ(kExpectedDefaultPermittedPath, default_ns_config->permitted_paths());

  auto& ns_configs = config->namespace_configs();
  ASSERT_EQ(2U, ns_configs.size());

  // find second namespace
  const NamespaceConfig* ns_system = nullptr;
  for (auto& ns : ns_configs) {
    std::string ns_name = ns->name();
    ASSERT_TRUE(ns_name == "system" || ns_name == "default")
        << "unexpected ns name: " << ns->name();

    if (ns_name == "system") {
      ns_system = ns.get();
    }
  }

  ASSERT_TRUE(ns_system != nullptr) << "system namespace was not found";

  ASSERT_TRUE(ns_system->isolated());
  ASSERT_EQ(kExpectedSystemSearchPath, ns_system->search_paths());
  ASSERT_EQ(kExpectedSystemPermittedPath, ns_system->permitted_paths());
}

TEST(linker_config, asan_smoke) {
#if defined(__LP64__)
  static const std::vector<std::string> kExpectedDefaultSearchPath = {
      "/data",
      "/vendor/lib64",
  };

  static const std::vector<std::string> kExpectedDefaultPermittedPath = {
      "/data",
      "/vendor",
  };

  static const std::vector<std::string> kExpectedSystemSearchPath = {
      "/data",
      "/system/lib64",
  };

  static const std::vector<std::string> kExpectedSystemPermittedPath = {
      "/data",
      "/system",
  };
#else
  static const std::vector<std::string> kExpectedDefaultSearchPath = {
      "/data",
      "/vendor/lib",
  };

  static const std::vector<std::string> kExpectedDefaultPermittedPath = {
      "/data",
      "/vendor",
  };

  static const std::vector<std::string> kExpectedSystemSearchPath = {
      "/data",
      "/system/lib",
  };

  static const std::vector<std::string> kExpectedSystemPermittedPath = {
      "/data",
      "/system",
  };
#endif

  TemporaryFile tmp_file;
  close(tmp_file.fd);
  tmp_file.fd = -1;

  android::base::WriteStringToFile(config_str, tmp_file.path);


  // read config
  const BinaryConfig* config = nullptr;
  std::string error_msg;
  ASSERT_TRUE(BinaryConfig::read_binary_config(tmp_file.path,
                                               "/some/test/dir/113/binary",
                                               true,
                                               &config,
                                               &error_msg));
  ASSERT_TRUE(config != nullptr);
  ASSERT_TRUE(error_msg.empty());

  ASSERT_EQ(113U, config->target_sdk_version());

  const NamespaceConfig* default_ns_config = config->default_namespace_config();
  ASSERT_TRUE(default_ns_config != nullptr);

  ASSERT_TRUE(default_ns_config->isolated());
  ASSERT_EQ(kExpectedDefaultSearchPath, default_ns_config->search_paths());
  ASSERT_EQ(kExpectedDefaultPermittedPath, default_ns_config->permitted_paths());

  auto& ns_configs = config->namespace_configs();
  ASSERT_EQ(2U, ns_configs.size());

  // find second namespace
  const NamespaceConfig* ns_system = nullptr;
  for (auto& ns : ns_configs) {
    std::string ns_name = ns->name();
    ASSERT_TRUE(ns_name == "system" || ns_name == "default")
        << "unexpected ns name: " << ns->name();

    if (ns_name == "system") {
      ns_system = ns.get();
    }
  }

  ASSERT_TRUE(ns_system != nullptr) << "system namespace was not found";

  ASSERT_TRUE(ns_system->isolated());
  ASSERT_EQ(kExpectedSystemSearchPath, ns_system->search_paths());
  ASSERT_EQ(kExpectedSystemPermittedPath, ns_system->permitted_paths());
}
