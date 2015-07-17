/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include <stdbool.h>
#include <android/dlext.h>

// These are stubs for functions that are actually defined
// in the dynamic linker and hijacked at runtime.

void* __android_dlopen_ext_impl(const char* filename __unused, int flag __unused,
                                const android_dlextinfo* extinfo __unused, void* dso __unused) {
  return 0;
}

__LIBC_HIDDEN__ void* __android_dlopen_ext(const char* filename, int flag,
                                           const android_dlextinfo* extinfo, void* dso) {
  return __android_dlopen_ext_impl(filename, flag, extinfo, dso);
}

void* __android_dlopen_impl(const char* filename __unused, int flag __unused, void* dso __unused) {
  return 0;
}

__LIBC_HIDDEN__ void* __android_dlopen(const char* filename, int flag, void* dso) {
  return __android_dlopen_impl(filename, flag, dso);
}

const char* dlerror(void) {
  return 0;
}

void* __android_dlsym_impl(void* handle __unused, const char* symbol __unused, void* dso __unused) {
  return 0;
}

__LIBC_HIDDEN__ void* __android_dlsym(void* handle, const char* symbol, void* dso) {
  return __android_dlsym_impl(handle, symbol, dso);
}


int dladdr(const void* addr __unused, Dl_info* info __unused) { return 0; }
int dlclose(void* handle __unused) { return 0; }

#if defined(__arm__)
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc __unused, int* pcount __unused) { return 0; }
#endif

int dl_iterate_phdr(int (*cb)(struct dl_phdr_info* info, size_t size, void* data) __unused, void* data __unused) { return 0; }

void android_get_LD_LIBRARY_PATH(char* buffer __unused, size_t buffer_size __unused) { }
void android_update_LD_LIBRARY_PATH(const char* ld_library_path __unused) { }

void android_set_application_target_sdk_version(uint32_t target __unused) { }
uint32_t android_get_application_target_sdk_version() { return 0; }
