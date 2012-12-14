#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

bionic_host_local_cflags := \
    -nostdinc -nostdlib -ffreestanding \
    -isystem prebuilts/gcc/linux-x86/host/i686-linux-glibc2.7-4.4.3/lib/gcc/i686-linux/4.4.3/include/ \
    -isystem bionic/libc/arch-x86/include/ \
    -isystem bionic/libc/include/ \
    -isystem bionic/libm/include/ \
    -isystem bionic/libm/include/i387/ \
    -isystem bionic/libstdc++/include \
    -isystem bionic/libc/kernel/common \
    -isystem bionic/libc/kernel/arch-x86 \
    -m32 -fPIC \
    -include build/core/combo/include/arch/linux-x86/AndroidConfig.h \
    -O2 -g

bionic_host_local_ldflags := \
    -m32 \
    -nodefaultlibs \
#    -Wl,--no-allow-shlib-undefined

BUILD_BIONIC_HOST_SHARED_LIBRARY := bionic/bionic_host_shared_library.mk
BUILD_BIONIC_HOST_STATIC_LIBRARY := bionic/bionic_host_static_library.mk
