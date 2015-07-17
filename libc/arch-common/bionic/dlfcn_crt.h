/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <android/dlext.h>

extern void* __dso_handle;

extern void* __android_dlopen(const char* filename, int flag, void* dso);
extern void* __android_dlsym(void* handle, const char* symbol, void* dso);
extern void* __android_dlopen_ext(const char* filename, int flags,
                                  const android_dlextinfo* extinfo, void* dso);
// Libdl used to export these in previous versions, therefore they need
// to remain global for binary compatibility.
#ifndef _LIBDL
__attribute__ ((visibility ("hidden")))
#endif
void* dlopen(const char* filename, int flag) {
  return __android_dlopen(filename, flag, &__dso_handle);
}

#ifndef _LIBDL
__attribute__ ((visibility ("hidden")))
#endif
void* dlsym(void* handle, const char* symbol) {
  return __android_dlsym(handle, symbol, &__dso_handle);
}

#ifndef _LIBDL
__attribute__ ((visibility ("hidden")))
#endif
void* android_dlopen_ext(const char* filename, int flags,
                         const android_dlextinfo* extinfo) {
  return __android_dlopen_ext(filename, flags, extinfo, &__dso_handle);
}
