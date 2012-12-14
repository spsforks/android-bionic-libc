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

ifeq ($(HOST_OS)-$(HOST_ARCH),linux-x86)

LOCAL_NO_DEFAULT_COMPILER_FLAGS := true
LOCAL_CFLAGS += $(bionic_host_local_cflags)
LOCAL_LDFLAGS += $(bionic_host_local_ldflags)
LOCAL_ADDITIONAL_DEPENDENCIES += $(BUILD_BIONIC_HOST_STATIC_LIBRARY)
include $(BUILD_HOST_STATIC_LIBRARY)

endif
