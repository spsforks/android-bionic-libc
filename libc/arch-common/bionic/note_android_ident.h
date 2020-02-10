/*
 * Copyright (C) 2020 The Android Open Source Project
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

// This is used by the platform, by the NDK, and for host bionic.
__attribute__((__section__((".note.android.ident")), __used__)) static struct {
  int namesz;
  int descsz;
  int type;
#if defined(__ANDROID__)
  char name[8];
  // Bionic for the device includes the specific API level.
  int android_api;
#if defined(ABI_NDK_VERSION) && defined(ABI_NDK_BUILD_NUMBER)
  // The NDK includes detail about what NDK was used.
  char ndk_version[64];
  char ndk_build_number[64];
#endif
#else
  // Host bionic.
  char name[12];
#endif
} note = {
#if defined(__ANDROID__)
#if defined(ABI_NDK_VERSION) && defined(ABI_NDK_BUILD_NUMBER)
    8, 4 + 64 + 64, 1, {"Android"}, PLATFORM_SDK_VERSION,
    ABI_NDK_VERSION,
    ABI_NDK_BUILD_NUMBER,
#else
    8, 4, 1, {"Android"}, PLATFORM_SDK_VERSION,
#endif
#else
    12,
    0,
    1,
    {"LinuxBionic"},
#endif
};
