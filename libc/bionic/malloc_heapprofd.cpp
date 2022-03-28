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

#include <fnmatch.h>
#include <private/ScopedFd.h>

#include <platform/bionic/malloc.h>
#include <private/bionic_config.h>
#include <private/bionic_malloc_dispatch.h>
#include <sys/system_properties.h>

#include "gwp_asan_wrappers.h"
#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"
#include "malloc_limit.h"

// Installing heapprofd hooks is a multi step process, as outlined below.
//
// The incremental hooking and a dedicated task thread are used since we cannot
// do heavy work within a signal handler, or when blocking a malloc invocation.
//
// +--->+-------------+------------------+
// | +->+kInitialState+----------------+ |  malloc functions are not intercepted in any way.
// | |  +-------+-----+                | |
// | |          | HandleHeapprofd      | |
// | |          v Signal()             | |
// | |  +-------+----------------+     | |  currently installing the ephemeral hooks.
// | |  |kInstallingEphemeralHook|<--+ | |
// | |  +-------+----------------+   | | |
// | |          |                    | | |
// | |          v                    | | |
// | |  +-------+---------------+    | | |  ephemeral hooks are installed. on the first call to
// | |  |kEphemeralHookInstalled|    | | |  malloc these hooks spawn a thread that installs the
// | |  +-------+---------------+    A B C  heapprofd hooks.
// | |          | MallocInit         | | |
// | |          v HeapprofdHook ()   | | |
// | |  +-------+--------------+     | | |  first call to malloc happened. the hooks are reset to
// | +--|kRemovingEphemeralHook|     | | |  kInitialState.
// |    +----------------------+     | | |
// |                                 | | |
// |                                 | | |
// |    +---------------+            | | |  currently installing the heapprofd hook
// |    |kInstallingHook|<-----------|-+ |
// |    +-------+-------+            |   |
// |            |                    |   |
// |            v                    |   |
// |    +-------+------+             |   |  heapprofd hooks are installed. these forward calls to
// |    |kHookInstalled|-------------+   |  malloc / free / etc. to heapprofd_client.so.
// |    +-------+------+                 |
// |            | DispatchReset()        |
// |            v                        |
// |    +-------+---------+              |  currently resetting the hooks to default.
// |----+kUninstallingHook|              |
//      +-----------------+              |
//                                       |
//                                       |
//      +------------------+             |  malloc debug / malloc hooks are active. these take
//      |kIncompatibleHooks+<------------+  precendence over heapprofd, so heapprofd will not get
//      +------------------+                enabled. this is a terminal state.
//
//
// A) HandleHeapprofdSignal()
// B) HeapprofdInstallHooksAtInit() / InitHeapprofd()
// C) HeapprofdRememberHookConflict()
enum MallocHeapprofdState : uint8_t {
  kInitialState,
  kInstallingEphemeralHook,
  kEphemeralHookInstalled,
  kRemovingEphemeralHook,
  kInstallingHook,
  kHookInstalled,
  kUninstallingHook,
  kIncompatibleHooks
};

enum ModifyGlobalsMode {
  kWithLock,   // all calls to MaybeModifyGlobals with kWithLock will serialise. they can fail
               // due to a concurrent call with kWithoutLock.
  kWithoutLock // calls to MaybeModifyGlobals with kWithoutLock do not serialise. they can fail
               // due to concurrent calls with kWithoutLock or kWithLock.
};

// Provide mutual exclusion so no two threads try to modify the globals at the same time.
template <typename Fn>
bool MaybeModifyGlobals(ModifyGlobalsMode mode, Fn f) {
  bool success = false;
  if (mode == kWithLock) {
    pthread_mutex_lock(&gGlobalsMutateLock);
  }
  // As we have grabbed the mutex, the following condition should always hold, except
  // if we are currently running HandleHeapprofdSignal.
  if (!atomic_exchange(&gGlobalsMutating, true)) {
    f();
    success = true;
    atomic_store(&gGlobalsMutating, false);
  } else {
    error_log("%s: heapprofd client: concurrent modification.", getprogname());
  }
  if (mode == kWithLock) {
    pthread_mutex_unlock(&gGlobalsMutateLock);
  }
  return success;
}

extern "C" void* MallocInitHeapprofdHook(size_t);

static constexpr char kHeapprofdSharedLib[] = "heapprofd_client.so";
static constexpr char kHeapprofdPrefix[] = "heapprofd";

constexpr size_t kMaxCmdlineSize = 512;

// The handle returned by dlopen when previously loading the heapprofd
// hooks. nullptr if shared library has not been already been loaded.
static _Atomic (void*) gHeapprofdHandle = nullptr;
static _Atomic MallocHeapprofdState gHeapprofdState = kInitialState;

static ssize_t ReadProcSelfCmdline(char* buf, size_t buf_sz) {
  ScopedFd fd(TEMP_FAILURE_RETRY(open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC)));
  if (fd.get() == -1) {
    error_log("%s: Failed to open /proc/self/cmdline", getprogname());
    return -1;
  }
  ssize_t rd = TEMP_FAILURE_RETRY(read(fd.get(), buf, buf_sz - 1));
  if (rd == -1) {
    error_log("%s: Failed to read /proc/self/cmdline", getprogname());
    return -1;
  }
  buf[rd] = '\0';
  return rd;
}

// Returns a pointer into |cmdline| corresponding to the argv0 without any
// leading directories if the binary path is absolute. |cmdline_len| corresponds
// to the length of the cmdline string as read out of procfs as a C string -
// length doesn't include the final nul terminator, but it must be present at
// cmdline[cmdline_len]. Note that normally the string itself will contain nul
// bytes, as that's what the kernel uses to separate arguments.
//
// Function output examples:
// * /system/bin/adb\0--flag -> adb
// * adb -> adb
// * com.example.app -> com.example.app
static const char* FindBinaryName(const char* cmdline, size_t cmdline_len) {
  // Find the first nul byte that signifies the end of argv0. We might not find
  // one if the process rewrote its cmdline without nul separators, and/or the
  // cmdline didn't fully fit into our read buffer. In such cases, proceed with
  // the full string to do best-effort matching.
  const char* argv0_end =
      static_cast<const char*>(memchr(cmdline, '\0', cmdline_len));
  if (argv0_end == nullptr) {
    argv0_end = cmdline + cmdline_len;
  }
  // Find the last path separator of argv0, if it exists.
  const char* name_start = static_cast<const char*>(
      memrchr(cmdline, '/', static_cast<size_t>(argv0_end - cmdline)));
  if (name_start == nullptr) {
    name_start = cmdline;
  } else {
    name_start++;  // skip the separator
  }
  return name_start;
}

static bool MatchGlobPattern(const char* pattern, const char* cmdline, const char* binname) {
  if (pattern[0] == '/') {
    return fnmatch(pattern, cmdline, FNM_NOESCAPE) == 0;
  }
  return fnmatch(pattern, binname, FNM_NOESCAPE) == 0;
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
// GWP-ASan as the backing dispatch. Writes to this variable is atomically
// protected by MaybeModifyGlobals.
// Reads are not protected, so this is atomic. We cannot fail the call in
// MallocInitHeapprofdHook.
static _Atomic (const MallocDispatch*) gPreviousDefaultDispatchTable = nullptr;
static MallocDispatch gEphemeralDispatch;

void HandleHeapprofdSignal() {
  if (atomic_load(&gHeapprofdState) == kIncompatibleHooks) {
    error_log("%s: not enabling heapprofd, malloc_debug/malloc_hooks are enabled.", getprogname());
    return;
  }

  // We cannot grab the mutex here, as this is used in a signal handler.
  MaybeModifyGlobals(kWithoutLock, [] {
    MallocHeapprofdState expected = kInitialState;
    // If hooks are already installed, we still want to install ephemeral hooks to retrigger
    // heapprofd client initialization.
    MallocHeapprofdState expected2 = kHookInstalled;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected,
          kInstallingEphemeralHook)) {
      const MallocDispatch* default_dispatch = GetDefaultDispatchTable();

      // Below, we initialize heapprofd lazily by redirecting libc's malloc() to
      // call MallocInitHeapprofdHook, which spawns off a thread and initializes
      // heapprofd. During the short period between now and when heapprofd is
      // initialized, allocations may need to be serviced. There are three
      // possible configurations:

      if (DispatchIsGwpAsan(default_dispatch)) {
        //  1. GWP-ASan was installed. We should use GWP-ASan for everything but
        //  malloc() in the interim period before heapprofd is properly
        //  installed. After heapprofd is finished installing, we will use
        //  GWP-ASan as heapprofd's backing allocator to allow heapprofd and
        //  GWP-ASan to coexist.
        atomic_store(&gPreviousDefaultDispatchTable, default_dispatch);
        gEphemeralDispatch = *default_dispatch;
      } else {
        // Either,
        // 2. No malloc hooking has been done (heapprofd, GWP-ASan, etc.). In
        // this case, everything but malloc() should come from the system
        // allocator.
        //
        // or,
        //
        // 3. It may be possible at this point in time that heapprofd is
        // *already* the default dispatch, and when it was initialized there
        // was no default dispatch installed. As such we don't want to use
        // heapprofd as the backing store for itself (otherwise infinite
        // recursion occurs). We will use the system allocator functions. Note:
        // We've checked that no other malloc interceptors are being used by
        // validating `gHeapprofdIncompatibleHooks` above, so we don't need to
        // worry about that case here.
        atomic_store(&gPreviousDefaultDispatchTable, nullptr);
        gEphemeralDispatch = *NativeAllocatorDispatch();
      }
    } else if (expected == kEphemeralHookInstalled) {
      // Nothing to do here. The ephemeral hook was installed, but
      // MallocInitHeapprofdHook() was never called. Since the ephemeral hook
      // is already there, no need to reinstall it.
      return;
    } else if (atomic_compare_exchange_strong(&gHeapprofdState, &expected2,
                                              kInstallingEphemeralHook)) {
      // if we still have hook installed, we can reuse the previous
      // decision. THIS IS REQUIRED FOR CORRECTNESS, because otherwise the
      // following can happen
      // 1. Assume DispatchIsGwpAsan(default_dispatch)
      // 2. This function is ran, sets gPreviousDefaultDispatchTable to
      //    GWP ASan.
      // 3. The sessions ends, DispatchReset FAILS due to a race. Now
      //    heapprofd hooks are default dispatch.
      // 4. We re-enter this function later. If we did NOT look at the
      //    previously recorded gPreviousDefaultDispatchTable, we would
      //    incorrectly reach case 3. below.
      // 5. The session ends, DispatchReset now resets the hooks to the
      //    system allocator. This is incorrect.
      const MallocDispatch* prev_dispatch =
        atomic_load(&gPreviousDefaultDispatchTable);
      gEphemeralDispatch = prev_dispatch ? *prev_dispatch : *NativeAllocatorDispatch();
    } else {
      error_log("%s: heapprofd: failed to transition kInitialState -> kInstallingEphemeralHook. "
          "current state (possible race): %d", getprogname(), expected2);
      return;
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
    atomic_store(&gHeapprofdState, kEphemeralHookInstalled);
  });
  // Otherwise, we're racing against malloc_limit's enable logic (at most once
  // per process, and a niche feature). This is highly unlikely, so simply give
  // up if it does happen.
}

bool HeapprofdShouldLoad() {
  // heapprofd.startup sysprop tells this function whether there's any patterns
  // with which to compare our own cmdline.
  char property_value[PROP_VALUE_MAX];
  if (__system_property_get("heapprofd.startup", property_value) == 0) {
    return false;
  }

  // check that the value is numeric and positive
  char* endptr = nullptr;
  long num_patterns = strtol(property_value, &endptr, 10);
  if (property_value[0] == '\0' || *endptr != '\0' || num_patterns <= 0) {
    return false;
  }

  // TODO: continue using 512 bytes? atrace uses 4096, but that seems excessive
  char cmdline[kMaxCmdlineSize];
  ssize_t cmdline_sz = ReadProcSelfCmdline(cmdline, sizeof(cmdline));
  if (cmdline_sz <= 0) {
    return false;
  }
  const char* binname = FindBinaryName(cmdline, static_cast<size_t>(cmdline_sz));

  char pattern_propname[32];
  for (long i = 0; i < num_patterns; i++) {
    if (snprintf(pattern_propname, sizeof(pattern_propname), "heapprofd.startup.%ld", i) <= 0) {
      return false; // TODO: skip err checking?
    }
    if (__system_property_get(pattern_propname, property_value) == 0) {
      continue; // TODO: or return outright?
    }
    if (MatchGlobPattern(property_value, cmdline, binname)) {
      return true;
    }
  }
  return false;
}

void HeapprofdRememberHookConflict() {
  atomic_store(&gHeapprofdState, kIncompatibleHooks);
}

static void CommonInstallHooks(libc_globals* globals) {
  void* impl_handle = atomic_load(&gHeapprofdHandle);
  if (impl_handle == nullptr) {
    impl_handle = LoadSharedLibrary(kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table);
    if (impl_handle == nullptr) {
      return;
    }
    atomic_store(&gHeapprofdHandle, impl_handle);
  } else if (!InitSharedLibrary(impl_handle, kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table)) {
    return;
  }

  FinishInstallHooks(globals, nullptr, kHeapprofdPrefix);
}

void HeapprofdInstallHooksAtInit(libc_globals *globals) {
  // Before we set the new default_dispatch_table in FinishInstallHooks, save
  // the previous dispatch table. If DispatchReset() gets called later, we want
  // to be able to restore the dispatch. We're still under
  // MaybeModifyGlobals locks at this point.
  atomic_store(&gPreviousDefaultDispatchTable, GetDefaultDispatchTable());
  MaybeModifyGlobals(kWithoutLock, [globals] {
    MallocHeapprofdState expected = kInitialState;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kInstallingHook)) {
      CommonInstallHooks(globals);
      atomic_store(&gHeapprofdState, kHookInstalled);
    } else {
      error_log("%s: heapprofd: failed to transition kInitialState -> kInstallingHook. "
          "current state (possible race): %d", getprogname(), expected);
    }
  });
}

static void* InitHeapprofd(void*) {
  MaybeModifyGlobals(kWithLock, [] {
    MallocHeapprofdState expected = kInitialState;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kInstallingHook)) {
      __libc_globals.mutate([](libc_globals* globals) {
        CommonInstallHooks(globals);
      });
      atomic_store(&gHeapprofdState, kHookInstalled);
    } else {
      error_log("%s: heapprofd: failed to transition kInitialState -> kInstallingHook. "
          "current state (possible race): %d", getprogname(), expected);
    }
  });
  return nullptr;
}

extern "C" void* MallocInitHeapprofdHook(size_t bytes) {
  MaybeModifyGlobals(kWithLock, [] {
    MallocHeapprofdState expected = kEphemeralHookInstalled;
    if (atomic_compare_exchange_strong(&gHeapprofdState, &expected, kRemovingEphemeralHook)) {
      __libc_globals.mutate([](libc_globals* globals) {
        const MallocDispatch* previous_dispatch = atomic_load(&gPreviousDefaultDispatchTable);
        atomic_store(&globals->default_dispatch_table, previous_dispatch);
        if (!MallocLimitInstalled()) {
          atomic_store(&globals->current_dispatch_table, previous_dispatch);
        }
      });
      atomic_store(&gHeapprofdState, kInitialState);

      pthread_t thread_id;
      if (pthread_create(&thread_id, nullptr, InitHeapprofd, nullptr) != 0) {
        error_log("%s: heapprofd: failed to pthread_create.", getprogname());
      } else if (pthread_setname_np(thread_id, "heapprofdinit") != 0) {
        error_log("%s: heapprod: failed to pthread_setname_np", getprogname());
      } else if (pthread_detach(thread_id) != 0) {
        error_log("%s: heapprofd: failed to pthread_detach", getprogname());
      }
    } else {
      warning_log("%s: heapprofd: could not transition kEphemeralHookInstalled -> "
          "kRemovingEphemeralHook. current state (possible race): %d. this can be benign "
          "if two threads try this transition at the same time", getprogname(),
          expected);
    }
  });
  // If we had a previous dispatch table, use that to service the allocation,
  // otherwise fall back to the native allocator.
  // This could be modified by a concurrent HandleHeapprofdSignal, but that is
  // benign as we will dispatch to the ephemeral handler, which will then dispatch
  // to the underlying one.
  const MallocDispatch* previous_dispatch = atomic_load(&gPreviousDefaultDispatchTable);
  if (previous_dispatch) {
    return previous_dispatch->malloc(bytes);
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
  if (atomic_load(&gHeapprofdState) == kInitialState) {
    return true;
  }

  bool success = false;
  MaybeModifyGlobals(kWithLock, [&success] {
    MallocHeapprofdState expected = kHookInstalled;

    if(atomic_compare_exchange_strong(&gHeapprofdState, &expected, kUninstallingHook)){
      __libc_globals.mutate([](libc_globals* globals) {
        const MallocDispatch* previous_dispatch = atomic_load(&gPreviousDefaultDispatchTable);
        atomic_store(&globals->default_dispatch_table, previous_dispatch);
        if (!MallocLimitInstalled()) {
          atomic_store(&globals->current_dispatch_table, previous_dispatch);
        }
      });
      atomic_store(&gHeapprofdState, kInitialState);
      success = true;
    } else {
      error_log("%s: heapprofd: failed to transition kHookInstalled -> kUninstallingHook. "
          "current state (possible race): %d", getprogname(),
          expected);
    }
  });
  if (!success) {
    errno = EAGAIN;
  }
  return success;
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
