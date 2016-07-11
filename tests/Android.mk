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

LOCAL_PATH := $(call my-dir)

ifeq ($(HOST_OS)-$(HOST_ARCH),$(filter $(HOST_OS)-$(HOST_ARCH),linux-x86 linux-x86_64))

# -----------------------------------------------------------------------------
# Compile time tests.
# -----------------------------------------------------------------------------

# Some of these are intentionally using = instead of := since we need access to
# some variables not initialtized until we're in the build system.

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/file-check-cxx \
    | $(HOST_OUT_EXECUTABLES)/FileCheck$(HOST_EXECUTABLE_SUFFIX) \

LOCAL_CXX = $(LOCAL_PATH)/file-check-cxx \
    $(HOST_OUT_EXECUTABLES)/FileCheck \
    $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CXX) \
    GCC \

LOCAL_CLANG := false
LOCAL_MODULE := bionic-compile-time-tests-g++
LOCAL_CPPFLAGS := -Wall
# Disable color diagnostics so the warnings output matches the source
LOCAL_CPPFLAGS +=  -fdiagnostics-color=never
LOCAL_SRC_FILES := fortify_compilation_test.cpp
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(LOCAL_PATH)/Android.mk \
    $(LOCAL_PATH)/file-check-cxx \
    | $(HOST_OUT_EXECUTABLES)/FileCheck$(HOST_EXECUTABLE_SUFFIX) \

LOCAL_CXX := $(LOCAL_PATH)/file-check-cxx \
    $(HOST_OUT_EXECUTABLES)/FileCheck \
    $(LLVM_PREBUILTS_PATH)/clang++ \
    CLANG \

LOCAL_CLANG := true
LOCAL_MODULE := bionic-compile-time-tests-clang++
LOCAL_CPPFLAGS := -Wall
# Disable color diagnostics so the warnings output matches the source
LOCAL_CPPFLAGS += -fno-color-diagnostics
# FileCheck will error if there aren't any CLANG: lines in the file, but there
# don't appear to be any cases where clang _does_ emit warnings for sn?printf :(
LOCAL_SRC_FILES :=
include $(BUILD_STATIC_LIBRARY)

# -----------------------------------------------------------------------------
# Host glibc tests.
# -----------------------------------------------------------------------------

# gtest needs ANDROID_DATA/local/tmp for death test output.
# Make sure to create ANDROID_DATA/local/tmp if doesn't exist.
# Use the current target out directory as ANDROID_DATA.
# BIONIC_TEST_FLAGS is either empty or it comes from the user.
.PHONY: bionic-unit-tests-glibc-run
bionic-unit-tests-glibc-run: bionic-unit-tests-glibc
	mkdir -p $(TARGET_OUT_DATA)/local/tmp
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_ROOT=$(TARGET_OUT) \
		$(HOST_OUT)/nativetest64/bionic-unit-tests-glibc/bionic-unit-tests-glibc $(BIONIC_TEST_FLAGS)

# -----------------------------------------------------------------------------
# Run the unit tests built against x86 bionic on an x86 host.
# -----------------------------------------------------------------------------

include $(LOCAL_PATH)/../build/run-on-host.mk

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))

TEST_TIMEOUT := 0

# BIONIC_TEST_FLAGS is either empty or it comes from the user.
.PHONY: bionic-unit-tests-run-on-host32
bionic-unit-tests-run-on-host32: bionic-unit-tests bionic-prepare-run-on-host
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_DNS_MODE=local \
	ANDROID_ROOT=$(TARGET_OUT) \
		timeout $(TEST_TIMEOUT) \
		$(TARGET_OUT_DATA)/nativetest/bionic-unit-tests/bionic-unit-tests $(BIONIC_TEST_FLAGS)

ifeq ($(TARGET_IS_64_BIT),true)
# add target to run lp64 tests
.PHONY: bionic-unit-tests-run-on-host64
bionic-unit-tests-run-on-host64: bionic-unit-tests bionic-prepare-run-on-host
	ANDROID_DATA=$(TARGET_OUT_DATA) \
	ANDROID_DNS_MODE=local \
	ANDROID_ROOT=$(TARGET_OUT) \
		timeout $(TEST_TIMEOUT) \
		$(TARGET_OUT_DATA)/nativetest64/bionic-unit-tests/bionic-unit-tests $(BIONIC_TEST_FLAGS)
endif

endif # x86 x86_64
endif # linux-x86

include $(call first-makefiles-under,$(LOCAL_PATH))
