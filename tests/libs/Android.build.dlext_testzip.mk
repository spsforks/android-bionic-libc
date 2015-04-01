#
# Copyright (C) 2014 The Android Open Source Project
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
# Library used by dlext tests - zipped and aligned
# -----------------------------------------------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE := libdlext_test_fd_zipaligned
LOCAL_MODULE_SUFFIX := .zip
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE_PATH := $($(bionic_2nd_arch_prefix)TARGET_OUT_DATA_NATIVE_TESTS)/libdlext_test_fd
LOCAL_2ND_ARCH_VAR_PREFIX := $(bionic_2nd_arch_prefix)

# Use ARCH_MIPS_PAGE_SHIFT, if defined, to determine page size
ifeq ($(ARCH_MIPS_PAGE_SHIFT),)
ZIPALIGN_PAGE_SIZE := 4096
else ifeq ($(ARCH_MIPS_PAGE_SHIFT),12)
ZIPALIGN_PAGE_SIZE := 4096
else ifeq ($(ARCH_MIPS_PAGE_SHIFT),14)
ZIPALIGN_PAGE_SIZE := 16384
else
$(error "Unsupported ARCH_MIPS_PAGE_SHIFT value: $(ARCH_MIPS_PAGE_SHIFT)")
endif

include $(BUILD_SYSTEM)/base_rules.mk

my_shared_libs := \
  $($(bionic_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/libdlext_test_fd.so

$(LOCAL_BUILT_MODULE): PRIVATE_ALIGNMENT := $(ZIPALIGN_PAGE_SIZE)
$(LOCAL_BUILT_MODULE) : $(my_shared_libs) | $(ZIPALIGN)
	@echo "Zipalign $(PRIVATE_ALIGNMENT): $@"
	$(hide) rm -rf $(dir $@) && mkdir -p $(dir $@)/libdir
	$(hide) cp $^ $(dir $@)/libdir
	$(hide) (cd $(dir $@) && touch empty_file.txt && zip -rD0 $(notdir $@).unaligned empty_file.txt libdir/*.so)
	$(hide) $(ZIPALIGN) $(PRIVATE_ALIGNMENT) $@.unaligned $@
