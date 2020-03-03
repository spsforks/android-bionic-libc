/*
 * Copyright (C) 2019 The Android Open Source Project
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

#if defined(LIBC_STATIC)
#error This file should not be compiled for static targets.
#endif

#include <dlfcn.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <platform/bionic/malloc.h>
#include <private/bionic_config.h>
#include <private/bionic_malloc_dispatch.h>
#include <sys/system_properties.h>

#include "gwp_asan_wrappers.h"
#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"
#include "malloc_limit.h"

static constexpr char kHeapprofdSharedLib[] = "heapprofd_client.so";
static constexpr char kHeapprofdPrefix[] = "heapprofd";
static constexpr char kHeapprofdPropertyEnable[] = "heapprofd.enable";

// The handle returned by dlopen when previously loading the heapprofd
// hooks. nullptr if shared library has not been already been loaded.
static _Atomic (void*) gHeapprofdHandle = nullptr;

// Installing heapprofd hooks is a multi step process, as outlined below.
//
// The incremental hooking and a dedicated task thread are used since we cannot
// do heavy work within a signal handler, or when blocking a malloc invocation.
//
// +--->+-------------+------------------+
// | +->+kInitialState+----------------+ |  malloc functions are not intercepted in any way.
// | |  +-------+-----+                | | 
// | |          |                      | | 
// | |          v                      | | 
// | |  +-------+---------------+      | |  currently installing the ephemeral hooks. these ephemeral
// | |  |InstallingEphemeralHook|      | |  hooks spawn a thread on the first call to malloc. this
// | |  +-------+---------------+      | |  thread then 
// | |          |                      | | 
// | |          v                      | | 
// | |  +-------+---------------+      | |  ephemeral hooks are installed. on the first call to malloc
// | |  |kEphemeralHookInstalled+--+   | |  these hooks spawn a thread that installs the heapprofd
// | |  +-------+---------------+  |   | |  hooks.
// | |          |                  |   | | 
// | |          v                  |   | | 
// | |  +-------+--------------+   |   | |  first call to malloc happened. the hooks are reset to the
// | +--|kRemovingEphemeralHook|   |   | |  initial state (i.e. kInitialState
// |    +----------------------+   |   | | 
// |                               |   | |   
// |                               |   | |   
// |    +-------+-------+          |   | |  currently installing the heapprofd hook 
// |    |kInstallingHook|<---------|---+ |  
// |    +-------+-------+          |     |
// |            |                  |     |
// |            v                  |     |
// |    +-------+------+           |     |  heapprofd hooks are installed. these forward calls to
// |    |kHookInstalled|           |     |  malloc / free / etc. to heapprofd_client.so.
// |    +-------+------+           |     |
// |            |                  |     |
// |            v                  |     |
// |    +-------+---------+        |     |  currently resetting the hooks to default.
// |----+kUninstallingHook+<-------+     |
//      +-----------------+              |
//                                       |
//                                       |
//      +------------------+             |  malloc debug / malloc hooks are active. these take
//      |kIncompatibleHooks+-------------+  precendence over heapprofd, so heapprofd will not get enabled.
//      +------------------+                this is a terminal state.
//
enum MallocHeapprofdState : uint8_t {
  kInitialState,
  kInstallingEphemeralHook,  // need to set gGlobalsMutating before transfering into this state.
  kEphemeralHookInstalled,
  kRemovingEphemeralHook,
  kInstallingHook,  // need to set gGlobalsMutating  before ransfering into this state.
  kHookInstalled,
  kUninstallingHook,  // need to set gGlobalsMutating before transfering into this state.
  kIncompatibleHooks // the process has enabled malloc_debug or malloc_hooks, which are
                     // incompatible (and take precedence over) heapprofd.
};

static _Atomic MallocHeapprofdState gHeapprofdState = kInitialState;

extern "C" void* MallocInitHeapprofdHook(size_t);

constexpr char kHeapprofdProgramPropertyPrefix[] = "heapprofd.enable.";
constexpr size_t kHeapprofdProgramPropertyPrefixSize = sizeof(kHeapprofdProgramPropertyPrefix) - 1;
constexpr size_t kMaxCmdlineSize = 512;

static bool GetHeapprofdProgramProperty(char* data, size_t size) {
  if (size < kHeapprofdProgramPropertyPrefixSize) {
    error_log("%s: Overflow constructing heapprofd property", getprogname());
    return false;
  }
  memcpy(data, kHeapprofdProgramPropertyPrefix, kHeapprofdProgramPropertyPrefixSize);

  int fd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    error_log("%s: Failed to open /proc/self/cmdline", getprogname());
    return false;
  }
  char cmdline[kMaxCmdlineSize];
  ssize_t rd = read(fd, cmdline, sizeof(cmdline) - 1);
  close(fd);
  if (rd == -1) {
    error_log("%s: Failed to read /proc/self/cmdline", getprogname());
    return false;
  }
  cmdline[rd] = '\0';
  char* first_arg = static_cast<char*>(memchr(cmdline, '\0', rd));
  if (first_arg == nullptr) {
    error_log("%s: Overflow reading cmdline", getprogname());
    return false;
  }
  // For consistency with what we do with Java app cmdlines, trim everything
  // after the @ sign of the first arg.
  char* first_at = static_cast<char*>(memchr(cmdline, '@', rd));
  if (first_at != nullptr && first_at < first_arg) {
    *first_at = '\0';
    first_arg = first_at;
  }

  char* start = static_cast<char*>(memrchr(cmdline, '/', first_arg - cmdline));
  if (start == first_arg) {
    // The first argument ended in a slash.
    error_log("%s: cmdline ends in /", getprogname());
    return false;
  } else if (start == nullptr) {
    start = cmdline;
  } else {
    // Skip the /.
    start++;
  }

  size_t name_size = static_cast<size_t>(first_arg - start);
  if (name_size >= size - kHeapprofdProgramPropertyPrefixSize) {
    error_log("%s: overflow constructing heapprofd property.", getprogname());
    return false;
  }
  // + 1 to also copy the trailing null byte.
  memcpy(data + kHeapprofdProgramPropertyPrefixSize, start, name_size + 1);
  return true;
}

// Runtime triggering entry-point. Two possible call sites:
// * when receiving a profiling signal with a si_value indicating heapprofd.
// * when a Zygote child is marking itself as profileable, and there's a
//   matching profiling request for this process (in which case heapprofd client
//   is loaded synchronously).
// In both cases, the caller is responsible for verifying that the process is
// considered profileable.

// Previously installed default dispatch table, if it exists. This is used to
// load heapprofd properly when GWP-ASan was already installed. If GWP-ASan was
// already installed, heapprofd will take over the dispatch table, but will use
// GWP-ASan as the backing dispatch. This variable is atomically protected by
// gHeapprofdInitInProgress.
static const MallocDispatch* gPreviousDefaultDispatchTable = nullptr;
static MallocDispatch gEphemeralDispatch;

void HandleHeapprofdSignal() {
  if (atomic_load_explicit(&gHeapprofdState, memory_order_acquire) == kIncompatibleHooks) {
    error_log("%s: not enabling heapprofd, malloc_debug/malloc_hooks are enabled.", getprogname());
    return;
  }

  // Checking this variable is only necessary when this could conflict with
  // the change to enable the allocation limit. All other places will
  // not ever have a conflict modifying the globals.
  // We cannot grab the mutex here, as this is used in a signal handler.
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    MallocHeapprofdState expected = kInitialState;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected,
          kInstallingEphemeralHook)) {
      const MallocDispatch* default_dispatch = GetDefaultDispatchTable();

      // Below, we initialize heapprofd lazily by redirecting libc's malloc() to
      // call MallocInitHeapprofdHook, which spawns off a thread and initializes
      // heapprofd. During the short period between now and when heapprofd is
      // initialized, allocations may need to be serviced. There are three
      // possible configurations:

      if (default_dispatch == nullptr) {
        //  1. No malloc hooking has been done (heapprofd, GWP-ASan, etc.). In
        //  this case, everything but malloc() should come from the system
        //  allocator.
        gPreviousDefaultDispatchTable = nullptr;
        gEphemeralDispatch = *NativeAllocatorDispatch();
      } else if (DispatchIsGwpAsan(default_dispatch)) {
        //  2. GWP-ASan was installed. We should use GWP-ASan for everything but
        //  malloc() in the interim period before heapprofd is properly
        //  installed. After heapprofd is finished installing, we will use
        //  GWP-ASan as heapprofd's backing allocator to allow heapprofd and
        //  GWP-ASan to coexist.
        gPreviousDefaultDispatchTable = default_dispatch;
        gEphemeralDispatch = *default_dispatch;
      } else {
        // 3. It may be possible at this point in time that heapprofd is
        // *already* the default dispatch, and as such we don't want to use
        // heapprofd as the backing store for itself (otherwise infinite
        // recursion occurs). We will use the system allocator functions. Note:
        // We've checked that no other malloc interceptors are being used by
        // validating `gHeapprofdIncompatibleHooks` above, so we don't need to
        // worry about that case here.
        gPreviousDefaultDispatchTable = nullptr;
        gEphemeralDispatch = *NativeAllocatorDispatch();
      }

      // Now, replace the malloc function so that the next call to malloc() will
      // initialize heapprofd.
      gEphemeralDispatch.malloc = MallocInitHeapprofdHook;

      // And finally, install these new malloc-family interceptors.
      __libc_globals.mutate([](libc_globals* globals) {
        atomic_store(&globals->default_dispatch_table, &gEphemeralDispatch);
        if (!MallocLimitInstalled()) {
          atomic_store(&globals->current_dispatch_table, &gEphemeralDispatch);
        }
      });
    }
    atomic_store(&gGlobalsMutating, false);
  }
  // Otherwise, we're racing against malloc_limit's enable logic (at most once
  // per process, and a niche feature). This is highly unlikely, so simply give
  // up if it does happen.
}

bool HeapprofdShouldLoad() {
  // First check for heapprofd.enable. If it is set to "all", enable
  // heapprofd for all processes. Otherwise, check heapprofd.enable.${prog},
  // if it is set and not 0, enable heap profiling for this process.
  char property_value[PROP_VALUE_MAX];
  if (__system_property_get(kHeapprofdPropertyEnable, property_value) == 0) {
    return false;
  }
  if (strcmp(property_value, "all") == 0) {
    return true;
  }

  char program_property[kHeapprofdProgramPropertyPrefixSize + kMaxCmdlineSize];
  if (!GetHeapprofdProgramProperty(program_property,
                                   sizeof(program_property))) {
    return false;
  }
  if (__system_property_get(program_property, property_value) == 0) {
    return false;
  }
  return property_value[0] != '\0';
}

void HeapprofdRememberHookConflict() {
  atomic_store_explicit(&gHeapprofdState, kIncompatibleHooks, memory_order_release);
}

static void CommonInstallHooks(libc_globals* globals) {
  void* impl_handle = atomic_load(&gHeapprofdHandle);
  bool reusing_handle = impl_handle != nullptr;
  if (!reusing_handle) {
    impl_handle = LoadSharedLibrary(kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table);
    if (impl_handle == nullptr) {
      return;
    }
  } else if (!InitSharedLibrary(impl_handle, kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table)) {
    return;
  }

  // Before we set the new default_dispatch_table in FinishInstallHooks, save
  // the previous dispatch table. If DispatchReset() gets called later, we want
  // to be able to restore the dispatch. We're still under
  // gHeapprofdInitInProgress locks at this point.
  gPreviousDefaultDispatchTable = GetDefaultDispatchTable();

  if (FinishInstallHooks(globals, nullptr, kHeapprofdPrefix)) {
    atomic_store(&gHeapprofdHandle, impl_handle);
  } else if (!reusing_handle) {
    dlclose(impl_handle);
  }

}

void HeapprofdInstallHooksAtInit(libc_globals* globals) {
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    MallocHeapprofdState expected = kInitialState;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kInstallingHook)) {
      CommonInstallHooks(globals);
    }
    atomic_exchange(&gGlobalsMutating, false);
  }
}

static void* InitHeapprofd(void*) {
  pthread_mutex_lock(&gGlobalsMutateLock);
  // As we have grabbed the mutex, the following condition should always hold, except
  // if we are currently running HandleHeapprofdSignal.
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    MallocHeapprofdState expected = kInitialState;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kInstallingHook)) {
      __libc_globals.mutate([](libc_globals* globals) {
        CommonInstallHooks(globals);
      });

      // Allow to install hook again to re-initialize heap profiling after the
      // current session finished.
      atomic_store(&gHeapprofdState, kHookInstalled);
    }
    atomic_exchange(&gGlobalsMutating, false);
  }
  pthread_mutex_unlock(&gGlobalsMutateLock);
  return nullptr;
}

extern "C" void* MallocInitHeapprofdHook(size_t bytes) {
  pthread_mutex_lock(&gGlobalsMutateLock);
  // As we have grabbed the mutex, the following condition should always hold, except
  // if we are currently running HandleHeapprofdSignal.
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    MallocHeapprofdState expected = kRemovingEphemeralHook;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kInitialState)) {
      pthread_mutex_lock(&gGlobalsMutateLock);
      __libc_globals.mutate([](libc_globals* globals) {
        atomic_store(&globals->default_dispatch_table, gPreviousDefaultDispatchTable);
        if (!MallocLimitInstalled()) {
          atomic_store(&globals->current_dispatch_table, gPreviousDefaultDispatchTable);
        }
      });

      pthread_t thread_id;
      if (pthread_create(&thread_id, nullptr, InitHeapprofd, nullptr) != 0) {
        error_log("%s: heapprofd: failed to pthread_create.", getprogname());
      } else if (pthread_detach(thread_id) != 0) {
        error_log("%s: heapprofd: failed to pthread_detach", getprogname());
      }
      if (pthread_setname_np(thread_id, "heapprofdinit") != 0) {
        error_log("%s: heapprod: failed to pthread_setname_np", getprogname());
      }
    }
  }
  pthread_mutex_unlock(&gGlobalsMutateLock);
  // If we had a previous dispatch table, use that to service the allocation,
  // otherwise fall back to the native allocator.
  // `gPreviousDefaultDispatchTable` won't change underneath us, as it's
  // protected by the `gHeapProfdInitInProgress` lock (which we currently hold).
  // The lock was originally taken by our caller in `HandleHeapprofdSignal()`,
  // and will be released by `CommonInstallHooks()` via. our `InitHeapprofd()`
  // thread that we just created.
  if (gPreviousDefaultDispatchTable) {
    return gPreviousDefaultDispatchTable->malloc(bytes);
  }
  return NativeAllocatorDispatch()->malloc(bytes);
}

bool HeapprofdInitZygoteChildProfiling() {
  // Conditionally start "from startup" profiling.
  if (HeapprofdShouldLoad()) {
    // Directly call the signal handler codepath (properly protects against
    // concurrent invocations).
    HandleHeapprofdSignal();
  }
  return true;
}

static bool DispatchReset() {
  MallocHeapprofdState expected = kHookInstalled;
  MallocHeapprofdState expected2 = kEphemeralHookInstalled;

  pthread_mutex_lock(&gGlobalsMutateLock);
  // As we have grabbed the mutex, the following condition should always hold, except
  // if we are currently running HandleHeapprofdSignal.
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    if(atomic_compare_exchange_strong(&gHeapprofdState, &expected, kUninstallingHook) ||
       atomic_compare_exchange_strong(&gHeapprofdState, &expected2, kUninstallingHook)) {
      __libc_globals.mutate([](libc_globals* globals) {
        atomic_store(&globals->default_dispatch_table, gPreviousDefaultDispatchTable);
        if (!MallocLimitInstalled()) {
          atomic_store(&globals->current_dispatch_table, gPreviousDefaultDispatchTable);
        }
      });
      atomic_store(&gHeapprofdState, kInitialState);
      return true;
    }
    atomic_exchange(&gGlobalsMutating, false);
  }
  pthread_mutex_unlock(&gGlobalsMutateLock);
  errno = EAGAIN;
  return false;
}

bool HeapprofdMallopt(int opcode, void* arg, size_t arg_size) {
  if (opcode == M_RESET_HOOKS) {
    if (arg != nullptr || arg_size != 0) {
      errno = EINVAL;
      return false;
    }
    return DispatchReset();
  }
  errno = ENOTSUP;
  return false;
}
