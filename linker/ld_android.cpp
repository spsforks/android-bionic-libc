/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "sys/cdefs.h"

// poor man's abort()
extern "C" void __fail() {
  void (*zero)() = nullptr;
  zero();
}

__strong_alias(__loader_android_create_namespace, __fail);
__strong_alias(__loader_android_dlopen_ext, __fail);
__strong_alias(__loader_android_dlwarning, __fail);
__strong_alias(__loader_android_get_application_target_sdk_version, __fail);
__strong_alias(__loader_android_get_LD_LIBRARY_PATH, __fail);
__strong_alias(__loader_android_get_exported_namespace, __fail);
__strong_alias(__loader_android_init_anonymous_namespace, __fail);
__strong_alias(__loader_android_link_namespaces, __fail);
__strong_alias(__loader_android_set_application_target_sdk_version, __fail);
__strong_alias(__loader_android_update_LD_LIBRARY_PATH, __fail);
__strong_alias(__loader_cfi_fail, __fail);
__strong_alias(__loader_dl_iterate_phdr, __fail);
__strong_alias(__loader_dladdr, __fail);
__strong_alias(__loader_dlclose, __fail);
__strong_alias(__loader_dlerror, __fail);
__strong_alias(__loader_dlopen, __fail);
__strong_alias(__loader_dlsym, __fail);
__strong_alias(__loader_dlvsym, __fail);
#if defined(__arm__)
__strong_alias(__loader_dl_unwind_find_exidx, __fail);
#endif
__strong_alias(rtld_db_dlactivity, __fail);

