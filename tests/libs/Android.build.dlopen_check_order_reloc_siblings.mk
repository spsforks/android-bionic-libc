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

# -----------------------------------------------------------------------------
# Libraries used by dlfcn tests to verify correct relocation order:
# libtest_check_order_reloc_siblings*.so
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# ..._1.so - this one has the correct answer
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_1_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_1_cflags := -D__ANSWER=42
libtest_check_order_reloc_siblings_1_shared_libraries := \
    libtest_check_order_reloc_siblings_a \
    libtest_check_order_reloc_siblings_b

module := libtest_check_order_reloc_siblings_1
include $(LOCAL_PATH)/Android.build.testlib.mk


# -----------------------------------------------------------------------------
# ..._2.so - implementing .. get_answer();
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_2_src_files := \
    dlopen_check_order_reloc_answer.cpp

libtest_check_order_reloc_siblings_2_shared_libraries := \
    libtest_check_order_reloc_siblings_c

module := libtest_check_order_reloc_siblings_2
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._a.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_a_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_a_cflags := -D__ANSWER=1
module := libtest_check_order_reloc_siblings_a
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._b.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_b_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_b_cflags := -D__ANSWER=2
module := libtest_check_order_reloc_siblings_b
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# ..._c.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_c_src_files := \
    dlopen_check_order_reloc_answer_impl.cpp

libtest_check_order_reloc_siblings_c_cflags := -D__ANSWER=3
module := libtest_check_order_reloc_siblings_c
include $(LOCAL_PATH)/Android.build.testlib.mk

# -----------------------------------------------------------------------------
# libtest_check_order_reloc_siblings.so
# -----------------------------------------------------------------------------
libtest_check_order_reloc_siblings_src_files := \
    empty.cpp

libtest_check_order_reloc_siblings_shared_libraries := \
    libtest_check_order_reloc_siblings_1 \
    libtest_check_order_reloc_siblings_2

module := libtest_check_order_reloc_siblings
include $(LOCAL_PATH)/Android.build.testlib.mk
