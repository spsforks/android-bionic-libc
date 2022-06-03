# Copyright 2022 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License

load("//build/bazel/rules/apis:cc_api_contribution.bzl", "cc_api_headers")

package(default_visibility = ["//bionic/libc:__pkg__"])

cc_api_headers(
  name="libc_kernel_uapi_headers",
  hdrs=glob(["**/*.h"]),
  system=True,
)

cc_api_headers(
  name="libc_kernal_uapi_asm_arm_headers",
  include_dir="asm-arm",
  hdrs=glob(["asm-arm/**/*.h"]),
  system=True,
  arch="arm",
)

cc_api_headers(
  name="libc_kernal_uapi_asm_arm64_headers",
  include_dir="asm-arm64",
  hdrs=glob(["asm-arm64/**/*.h"]),
  system=True,
  arch="arm64",
)

cc_api_headers(
  name="libc_kernal_uapi_asm_x86_headers",
  include_dir="asm-x86",
  hdrs=glob(["asm-x86/**/*.h"]),
  system=True,
  arch="x86",
)

cc_api_headers(
  name="libc_kernal_uapi_asm_x86_64_headers",
  include_dir="asm-x86_64",
  hdrs=glob(["asm-x86_64/**/*.h"]),
  system=True,
  arch="x86_64",
)
