#
# Copyright (C) 2008 The Android Open Source Project
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

# Use this flag to distinguish between building bionic and users of bionic.
# This is so that the override file bionic_internal_symbols.h will
# only activate INSIDE bionic, and not elsewhere.
# Other packages that use bionic are free to use the external API (e.g. malloc())
# instead of the internal symbol (e.g. __bionic_malloc())
BIONIC_INTERNAL:=-D__BIONIC_INTERNAL__=1

include $(call all-subdir-makefiles)
