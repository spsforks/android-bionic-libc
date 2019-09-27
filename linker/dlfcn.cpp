/*
 * Copyright (C) 2007 The Android Open Source Project
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

#include "linker.h"
#include "linker_cfi.h"
#include "linker_globals.h"
#include "linker_dlwarning.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/api-level.h>

#include <bionic/pthread_internal.h>
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "private/ScopedPthreadMutexLocker.h"

#define __LINKER_PUBLIC__ __attribute__((visibility("default")))

extern "C" {

android_namespace_t* __loader_android_create_namespace(const char* name,
                                                       const char* ld_library_path,
                                                       const char* default_library_path,
                                                       uint64_t type,
                                                       const char* permitted_when_isolated_path,
                                                       android_namespace_t* parent_namespace,
                                                       const void* caller_addr) __LINKER_PUBLIC__;
void* __loader_android_dlopen_ext(const char* filename,
                           int flags,
                           const android_dlextinfo* extinfo,
                           const void* caller_addr) __LINKER_PUBLIC__;
void __loader_android_dlwarning(void* obj, void (*f)(void*, const char*)) __LINKER_PUBLIC__;
int __loader_android_get_application_target_sdk_version() __LINKER_PUBLIC__;
void __loader_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) __LINKER_PUBLIC__;
android_namespace_t* __loader_android_get_exported_namespace(const char* name) __LINKER_PUBLIC__;
bool __loader_android_init_anonymous_namespace(const char* shared_libs_sonames,
                                               const char* library_search_path) __LINKER_PUBLIC__;
bool __loader_android_link_namespaces(android_namespace_t* namespace_from,
                                      android_namespace_t* namespace_to,
                                      const char* shared_libs_sonames) __LINKER_PUBLIC__;
bool __loader_android_link_namespaces_all_libs(android_namespace_t* namespace_from,
                                               android_namespace_t* namespace_to) __LINKER_PUBLIC__;
void __loader_android_set_application_target_sdk_version(int target) __LINKER_PUBLIC__;
void __loader_android_update_LD_LIBRARY_PATH(const char* ld_library_path) __LINKER_PUBLIC__;
void __loader_cfi_fail(uint64_t CallSiteTypeId,
                       void* Ptr,
                       void *DiagData,
                       void *CallerPc) __LINKER_PUBLIC__;
int __loader_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data),
                             void* data) __LINKER_PUBLIC__;
int __loader_dladdr(const void* addr, Dl_info* info) __LINKER_PUBLIC__;
int __loader_dlclose(void* handle) __LINKER_PUBLIC__;
char* __loader_dlerror() __LINKER_PUBLIC__;
void* __loader_dlopen(const char* filename, int flags, const void* caller_addr) __LINKER_PUBLIC__;
void* __loader_dlsym(void* handle, const char* symbol, const void* caller_addr) __LINKER_PUBLIC__;
void* __loader_dlvsym(void* handle,
                      const char* symbol,
                      const char* version,
                      const void* caller_addr) __LINKER_PUBLIC__;
void __loader_add_thread_local_dtor(void* dso_handle) __LINKER_PUBLIC__;
void __loader_remove_thread_local_dtor(void* dso_handle) __LINKER_PUBLIC__;
libc_shared_globals* __loader_shared_globals() __LINKER_PUBLIC__;
#if defined(__arm__)
_Unwind_Ptr __loader_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) __LINKER_PUBLIC__;
#endif
}

enum class LoaderLockLevel {
  UNLOCKED,
  SHARED,
  EXCLUSIVE,
};

static pthread_rwlock_t g_dl_rwlock = PTHREAD_RWLOCK_INITIALIZER;
static atomic_int g_dl_user_thread;
// Modified only by the g_dl_user_thread thread
static LoaderLockLevel g_dl_rwlock_level = LoaderLockLevel::UNLOCKED;

static int get_user_thread_tid() {
  return atomic_load_explicit(&g_dl_user_thread, memory_order_relaxed);
}

static void set_user_thread_tid(int tid) {
  atomic_store_explicit(&g_dl_user_thread, tid, memory_order_relaxed);
}

static void transition_loader_lock(LoaderLockLevel from, LoaderLockLevel to)
{
  const char* fn_name;
  int ret;
  switch (from) {
    case LoaderLockLevel::UNLOCKED:
      switch (to) {
        case LoaderLockLevel::UNLOCKED:
          fn_name = "noop";
          ret = 0;
          break;
        case LoaderLockLevel::EXCLUSIVE:
          fn_name = "pthread_rwlock_wrlock";
          ret = pthread_rwlock_wrlock(&g_dl_rwlock);
          break;
        case LoaderLockLevel::SHARED:
          fn_name = "pthread_rwlock_rdlock";
          ret = pthread_rwlock_rdlock(&g_dl_rwlock);
          break;
      }
      break;
    case LoaderLockLevel::EXCLUSIVE:
      switch (to) {
        case LoaderLockLevel::UNLOCKED:
          fn_name = "pthread_rwlock_unlock";
          ret = pthread_rwlock_unlock(&g_dl_rwlock);
          break;
        case LoaderLockLevel::EXCLUSIVE:
          fn_name = "noop";
          ret = 0;
          break;
        case LoaderLockLevel::SHARED:
          fn_name = "pthread_rwlock_downgrade_wrlock_to_rdlock_np";
          ret = pthread_rwlock_downgrade_wrlock_to_rdlock_np(&g_dl_rwlock);
          break;
      }
      break;
    case LoaderLockLevel::SHARED:
      switch (to) {
        case LoaderLockLevel::UNLOCKED:
          fn_name = "pthread_rwlock_unlock";
          ret = pthread_rwlock_unlock(&g_dl_rwlock);
          break;
        case LoaderLockLevel::EXCLUSIVE: {
          if (get_user_thread_tid() != __get_thread()->tid) {
            async_safe_fatal("cannot upgrade rwlock of non-current linker thread");
          }
          fn_name = "pthread_rwlock_upgrade_rdlock_to_wrlock_np";
          ret = pthread_rwlock_upgrade_rdlock_to_wrlock_np(
              &g_dl_rwlock, CLOCK_MONOTONIC, nullptr);
          break;
        }
        case LoaderLockLevel::SHARED:
          fn_name = "noop";
          ret = 0;
          break;
      }
      break;
  }
  if (ret) {
    async_safe_fatal("%s failed: %d", fn_name, ret);
  }
}

// Layer on top of rwlock that lets us take the lock exclusively recursively and automatically
// upgrade back to exclusive if we've downgraded.
class ScopedLoaderLock {
 public:
  explicit ScopedLoaderLock(LoaderLockLevel min_level) {
    if (min_level < LoaderLockLevel::SHARED) {
      min_level = LoaderLockLevel::SHARED;
    }
    int tid = __get_thread()->tid;
    if (get_user_thread_tid() != tid) {
      old_level_ = LoaderLockLevel::UNLOCKED;
      transition_loader_lock(old_level_, min_level);  // Blocks
      if (min_level == LoaderLockLevel::EXCLUSIVE) {
        set_user_thread_tid(tid);
        g_dl_rwlock_level = min_level;
      }
    } else {
      old_level_ = g_dl_rwlock_level;
      // Don't let ScopedLoaderLock reduce the current protection level.
      if (min_level < old_level_) {
        min_level = old_level_;
      }
      transition_loader_lock(old_level_, min_level);  // Blocks
      g_dl_rwlock_level = min_level;
    }
    level_ = min_level;
  }

  // Must be the "top" ScopedLoaderLock
  void downgrade_to(LoaderLockLevel to) {
    int tid = __get_thread()->tid;
    if (to < level_ && get_user_thread_tid() == tid) {
      transition_loader_lock(level_, to);
      level_ = to;
      g_dl_rwlock_level = to;
    }
  }

  ~ScopedLoaderLock() {
    int tid = __get_thread()->tid;
    if (get_user_thread_tid() == tid) {
      g_dl_rwlock_level = old_level_;
      if (old_level_ == LoaderLockLevel::UNLOCKED) {
        set_user_thread_tid(0);
      }
    }
    transition_loader_lock(level_, old_level_);
  }

 private:
  LoaderLockLevel old_level_;
  LoaderLockLevel level_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(ScopedLoaderLock);
};

static char* __bionic_set_dlerror(char* new_value) {
  char* old_value = __get_thread()->current_dlerror;
  __get_thread()->current_dlerror = new_value;

  if (new_value != nullptr) LD_LOG(kLogErrors, "dlerror set to \"%s\"", new_value);
  return old_value;
}

static void __bionic_format_dlerror(const char* msg, const char* detail) {
  char* buffer = __get_thread()->dlerror_buffer;
  strlcpy(buffer, msg, __BIONIC_DLERROR_BUFFER_SIZE);
  if (detail != nullptr) {
    strlcat(buffer, ": ", __BIONIC_DLERROR_BUFFER_SIZE);
    strlcat(buffer, detail, __BIONIC_DLERROR_BUFFER_SIZE);
  }

  __bionic_set_dlerror(buffer);
}

char* __loader_dlerror() {
  char* old_value = __bionic_set_dlerror(nullptr);
  return old_value;
}

void __loader_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  do_android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void __loader_android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  do_android_update_LD_LIBRARY_PATH(ld_library_path);
}

static void before_call_constructors(void* data) {
  ScopedLoaderLock* sll = reinterpret_cast<ScopedLoaderLock*>(data);
  sll->downgrade_to(LoaderLockLevel::SHARED);
}

static void* dlopen_ext(const char* filename,
                        int flags,
                        const android_dlextinfo* extinfo,
                        const void* caller_addr) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  g_linker_logger.ResetState();
  void* result = do_dlopen(filename, flags, extinfo, caller_addr,
                           before_call_constructors, &locker);
  if (result == nullptr) {
    __bionic_format_dlerror("dlopen failed", linker_get_error_buffer());
    return nullptr;
  }
  return result;
}

void* __loader_android_dlopen_ext(const char* filename,
                           int flags,
                           const android_dlextinfo* extinfo,
                           const void* caller_addr) {
  return dlopen_ext(filename, flags, extinfo, caller_addr);
}

void* __loader_dlopen(const char* filename, int flags, const void* caller_addr) {
  return dlopen_ext(filename, flags, nullptr, caller_addr);
}

void* dlsym_impl(void* handle, const char* symbol, const char* version, const void* caller_addr) {
  void* result;
  SymbolLookupResult ret;

  if (!LinkerLogger::isGlobalDisabled()) {
    // If the linker logger is enabled, we need to globally serialize.
    ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
    g_linker_logger.ResetState();
    ret = do_dlsym(handle, symbol, version, caller_addr, &result);
  } else {
    {
      ScopedLoaderLock locker(LoaderLockLevel::SHARED);
      ret = do_dlsym(handle, symbol, version, caller_addr, &result);
    }
    // The thread doing the linking is allowed to see uninitialized symbols.
    if (ret == SymbolLookupResult::SUCCESS_BUT_CONSTRUCTORS_RUNNING &&
        get_user_thread_tid() != __get_thread()->tid) {
      // Taking the loader lock exclusive here is sufficient for ensuring that any constructors
      // running at symbol lookup time have now completed.
      ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
    }
  }

  if (ret == SymbolLookupResult::FAILURE) {
    __bionic_format_dlerror(linker_get_error_buffer(), nullptr);
    return nullptr;
  }

  return result;
}

void* __loader_dlsym(void* handle, const char* symbol, const void* caller_addr) {
  return dlsym_impl(handle, symbol, nullptr, caller_addr);
}

void* __loader_dlvsym(void* handle, const char* symbol, const char* version, const void* caller_addr) {
  return dlsym_impl(handle, symbol, version, caller_addr);
}

int __loader_dladdr(const void* addr, Dl_info* info) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  return do_dladdr(addr, info);
}

int __loader_dlclose(void* handle) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  int result = do_dlclose(handle);
  if (result != 0) {
    __bionic_format_dlerror("dlclose failed", linker_get_error_buffer());
  }
  return result;
}

int __loader_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  return do_dl_iterate_phdr(cb, data);
}

// This function is needed by libgcc.a
int dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  return __loader_dl_iterate_phdr(cb, data);
}

#if defined(__arm__)
_Unwind_Ptr __loader_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  return do_dl_unwind_find_exidx(pc, pcount);
}
#endif

void __loader_android_set_application_target_sdk_version(int target) {
  // lock to avoid modification in the middle of dlopen.
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  set_application_target_sdk_version(target);
}

int __loader_android_get_application_target_sdk_version() {
  return get_application_target_sdk_version();
}

void __loader_android_dlwarning(void* obj, void (*f)(void*, const char*)) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  get_dlwarning(obj, f);
}

bool __loader_android_init_anonymous_namespace(const char* shared_libs_sonames,
                                               const char* library_search_path) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  bool success = init_anonymous_namespace(shared_libs_sonames, library_search_path);
  if (!success) {
    __bionic_format_dlerror("android_init_anonymous_namespace failed", linker_get_error_buffer());
  }

  return success;
}

android_namespace_t* __loader_android_create_namespace(const char* name,
                                                const char* ld_library_path,
                                                const char* default_library_path,
                                                uint64_t type,
                                                const char* permitted_when_isolated_path,
                                                android_namespace_t* parent_namespace,
                                                const void* caller_addr) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);

  android_namespace_t* result = create_namespace(caller_addr,
                                                 name,
                                                 ld_library_path,
                                                 default_library_path,
                                                 type,
                                                 permitted_when_isolated_path,
                                                 parent_namespace);

  if (result == nullptr) {
    __bionic_format_dlerror("android_create_namespace failed", linker_get_error_buffer());
  }

  return result;
}

bool __loader_android_link_namespaces(android_namespace_t* namespace_from,
                                      android_namespace_t* namespace_to,
                                      const char* shared_libs_sonames) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);

  bool success = link_namespaces(namespace_from, namespace_to, shared_libs_sonames);

  if (!success) {
    __bionic_format_dlerror("android_link_namespaces failed", linker_get_error_buffer());
  }

  return success;
}

bool __loader_android_link_namespaces_all_libs(android_namespace_t* namespace_from,
                                               android_namespace_t* namespace_to) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);

  bool success = link_namespaces_all_libs(namespace_from, namespace_to);

  if (!success) {
    __bionic_format_dlerror("android_link_namespaces_all_libs failed", linker_get_error_buffer());
  }

  return success;
}

android_namespace_t* __loader_android_get_exported_namespace(const char* name) {
  return get_exported_namespace(name);
}

void __loader_cfi_fail(uint64_t CallSiteTypeId, void* Ptr, void *DiagData, void *CallerPc) {
  CFIShadowWriter::CfiFail(CallSiteTypeId, Ptr, DiagData, CallerPc);
}

void __loader_add_thread_local_dtor(void* dso_handle) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  increment_dso_handle_reference_counter(dso_handle);
}

void __loader_remove_thread_local_dtor(void* dso_handle) {
  ScopedLoaderLock locker(LoaderLockLevel::EXCLUSIVE);
  decrement_dso_handle_reference_counter(dso_handle);
}

libc_shared_globals* __loader_shared_globals() {
  return __libc_shared_globals();
}

static uint8_t __libdl_info_buf[sizeof(soinfo)] __attribute__((aligned(8)));
static soinfo* __libdl_info = nullptr;

// This is used by the dynamic linker. Every process gets these symbols for free.
soinfo* get_libdl_info(const soinfo& linker_si) {
  CHECK((linker_si.flags_ & FLAG_GNU_HASH) != 0);

  if (__libdl_info == nullptr) {
    __libdl_info = new (__libdl_info_buf) soinfo(&g_default_namespace, nullptr, nullptr, 0, 0);
    __libdl_info->flags_ |= (FLAG_LINKED | FLAG_GNU_HASH);
    __libdl_info->strtab_ = linker_si.strtab_;
    __libdl_info->symtab_ = linker_si.symtab_;
    __libdl_info->load_bias = linker_si.load_bias;
    __libdl_info->phdr = linker_si.phdr;
    __libdl_info->phnum = linker_si.phnum;

    __libdl_info->gnu_nbucket_ = linker_si.gnu_nbucket_;
    __libdl_info->gnu_maskwords_ = linker_si.gnu_maskwords_;
    __libdl_info->gnu_shift2_ = linker_si.gnu_shift2_;
    __libdl_info->gnu_bloom_filter_ = linker_si.gnu_bloom_filter_;
    __libdl_info->gnu_bucket_ = linker_si.gnu_bucket_;
    __libdl_info->gnu_chain_ = linker_si.gnu_chain_;

    __libdl_info->ref_count_ = 1;
    __libdl_info->strtab_size_ = linker_si.strtab_size_;
    __libdl_info->local_group_root_ = __libdl_info;
    __libdl_info->soname_ = linker_si.soname_;
    __libdl_info->target_sdk_version_ = __ANDROID_API__;
    __libdl_info->generate_handle();
#if defined(__work_around_b_24465209__)
    strlcpy(__libdl_info->old_name_, __libdl_info->soname_, sizeof(__libdl_info->old_name_));
#endif
  }

  return __libdl_info;
}
