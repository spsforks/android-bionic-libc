/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <android/dlext.h>

// These are implemented in libdl.so
extern "C" void* __android_dlopen_ext_impl(const char* filename, int flags,
                                           const android_dlextinfo* extinfo, void* dso);
extern "C" void* __android_dlopen_impl(const char* filename, int flags, void* dso);
extern "C" void* __android_dlsym_impl(void* handle, const char* symbol, void* dso);

extern "C" void* __android_dlopen_ext(const char* filename, int flags,
                                      const android_dlextinfo* extinfo, void* dso) {
  return __android_dlopen_ext_impl(filename, flags, extinfo, dso);
}

extern "C" void* __android_dlopen(const char* filename, int flags, void* dso) {
  return __android_dlopen_impl(filename, flags, dso);
}

extern "C" void* __android_dlsym(void* handle, const char* symbol, void* dso) {
  return __android_dlsym_impl(handle, symbol, dso);
}
