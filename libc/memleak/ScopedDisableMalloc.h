/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef LIBMEMLEAK_SCOPED_DISABLE_MALLOC_H_
#define LIBMEMLEAK_SCOPED_DISABLE_MALLOC_H_

#include <memory>

#include "bionic/jemalloc.h"
#include "private/bionic_macros.h"
#include "private/libc_logging.h"

#include "log.h"
#include "ScopedAlarm.h"

class DisableMallocGuard{
 public:
  DisableMallocGuard() : old_tcache_(false), disabled_(false) {}
  ~DisableMallocGuard() {
    Enable();
  }

  void Disable() {
    if (!disabled_) {
      bool new_tcache = false;
      size_t old_len = sizeof(old_tcache_);
      int ret = je_mallctl("thread.tcache.enabled", &old_tcache_, &old_len,
          &new_tcache, sizeof(new_tcache));
      if (ret < 0) {
        log_fatal("failed to disable tcache");
      }
      je_jemalloc_prefork();
      disabled_ = true;
    }
  }

  void Enable() {
    if (disabled_) {
      je_jemalloc_postfork_parent();
      int ret = je_mallctl("thread.tcache.enabled", NULL, NULL,
          &old_tcache_, sizeof(old_tcache_));
      if (ret < 0) {
        log_fatal("failed to re-enable tcache");
      }
      disabled_ = false;
    }
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(DisableMallocGuard);
  bool old_tcache_;
  bool disabled_;
};

/* Any calls to malloc or free from this thread will deadlock as long as this
 * object is in scope.  Calls to malloc from other threads may succeed (for
 * example if the allocation is satisfied out of the thread's tcache), or may
 * block until the object is destroyed.
 *
 * Don't call fork() while malloc is disabled, it needs the same locks held
 * here.
 */
class ScopedDisableMalloc {
 public:
  ScopedDisableMalloc() {
    disable_malloc_.Disable();
  }

  ~ScopedDisableMalloc() {
    disable_malloc_.Enable();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedDisableMalloc);
  DisableMallocGuard disable_malloc_;
};

class ScopedDisableMallocTimeout {
 public:
  ScopedDisableMallocTimeout(std::chrono::milliseconds timeout = std::chrono::milliseconds(1000)) :
    timeout_(timeout), timed_out_(false), disable_malloc_() {
    Disable();
  }

  ~ScopedDisableMallocTimeout() {
    Enable();
  }

  bool timed_out() {
    return timed_out_;
  }

  void Enable() {
    disable_malloc_.Enable();
    alarm_ = nullptr;
  }

  void Disable() {
    /* set up the alarm before disabling malloc so unique_ptr can be used */
    alarm_ = std::make_unique<ScopedAlarm>(timeout_, [&]() {
      disable_malloc_.Enable();
      timed_out_ = true;
    });

    disable_malloc_.Disable();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedDisableMallocTimeout);
  std::chrono::milliseconds timeout_;
  bool timed_out_;
  std::unique_ptr<ScopedAlarm> alarm_;
  DisableMallocGuard disable_malloc_;
};

#endif // LIBMEMLEAK_SCOPED_DISABLE_MALLOC_H_
