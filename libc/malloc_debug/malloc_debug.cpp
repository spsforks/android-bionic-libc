/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <errno.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/param.h>
#include <unistd.h>

#include <private/bionic_macros.h>
#include <private/bionic_malloc_dispatch.h>
#include <private/bionic_malloc_debug.h>

#include "debug_backtrace.h"
#include "MallocDebugConfig.h"
#include "malloc_debug_disable.h"

struct Header {
  uint32_t tag;
  void* orig_pointer;
  size_t size;
  size_t usable_size;
} __attribute__((packed));

struct TrackHeader {
  Header* prev = nullptr;
  Header* next = nullptr;
} __attribute__((packed));

struct DebugData {
  size_t track = 0;
  size_t alloc_backtrace = 0;
  size_t free_backtrace = 0;
  size_t front_guard = 0;
  size_t pointer = 0;
  size_t extra_bytes = 0;
  bool need_header = false;

  void* front_guard_mem = nullptr;
  void* rear_guard_mem = nullptr;

  // Only used for tracking allocations and checking for memory leaks.
  pthread_mutex_t track_mutex = PTHREAD_MUTEX_INITIALIZER;
  Header* track_list = nullptr;
};

constexpr uint32_t DEBUG_TAG = 0x1ee7d00d;

DebugData g_debug;
pthread_key_t g_debug_calls_disabled;
static const MallocDispatch* g_dispatch;

static MallocDebugConfig g_config;

void* GetPointer(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<void*>(value + g_debug.pointer);
}

Header* GetHeader(void* pointer) {
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<Header*>(value - g_debug.pointer);
}

TrackHeader* GetTrackHeader(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<TrackHeader*>(value + g_debug.track);
}

uintptr_t* GetAllocBacktrace(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<uintptr_t*>(value + g_debug.alloc_backtrace);
}

uintptr_t* GetFreeBacktrace(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<uintptr_t*>(value + g_debug.free_backtrace);
}

uint8_t* GetFrontGuard(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<uint8_t*>(value + g_debug.front_guard);
}

uint8_t* GetRearGuard(Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(GetPointer(header));
  return reinterpret_cast<uint8_t*>(value + header->size);
}

extern "C" bool debug_initialize(const MallocDispatch* malloc_dispatch) {
  g_dispatch = malloc_dispatch;

  if (!g_config.SetFromProperties()) {
    return false;
  }

  if (pthread_key_create(&g_debug_calls_disabled, nullptr) != 0) {
    error_log("pthread_key_create failed: %s", strerror(errno));
    return false;
  }

  pthread_setspecific(g_debug_calls_disabled, nullptr);

  if (g_config.options & MallocDebugConfig::FRONT_GUARD) {
    // Create a buffer for fast comparisons of the front guard.
    g_debug.front_guard_mem = g_dispatch->malloc(g_config.front_guard_bytes);
    if (g_debug.front_guard_mem == nullptr) {
      return false;
    }
    memset(g_debug.front_guard_mem, g_config.front_guard_value, g_config.front_guard_bytes);
  } else {
    g_debug.front_guard_mem = nullptr;
  }
  if (g_config.options & MallocDebugConfig::REAR_GUARD) {
    // Create a buffer for fast comparisons of the rear guard.
    g_debug.rear_guard_mem = g_dispatch->malloc(g_config.rear_guard_bytes);
    if (g_debug.rear_guard_mem == nullptr) {
      return false;
    }
    memset(g_debug.rear_guard_mem, g_config.rear_guard_value, g_config.rear_guard_bytes);
  } else {
    g_debug.rear_guard_mem = nullptr;
  }
#if 0
  if (g_config.options & MallocDebugConfig::FILL_ON_FREE) {
    g_check_free.check_mem.size(CHECK_FREE_BYTES);
    memset(g_check_free.check_mem.data(), g_config.fill_alloc_value, CHECK_FREE_BYTES);
  }
#endif

  // Check to see if the options that require a header are enabled.
  if ((g_config.options & ~(MallocDebugConfig::NO_HEADER_OPTIONS)) != 0) {
    // Initialize all of the static header offsets.
    size_t offset = BIONIC_ALIGN(sizeof(Header), sizeof(uintptr_t));
    if (g_config.options & MallocDebugConfig::TRACK_ALLOC) {
      g_debug.track = offset;
      offset += BIONIC_ALIGN(sizeof(TrackHeader), sizeof(uintptr_t));
    } else {
      g_debug.track = static_cast<size_t>(-1);
    }
    if (g_config.options & MallocDebugConfig::BACKTRACE) {
      g_debug.alloc_backtrace = offset;
      offset += sizeof(uintptr_t) * g_config.backtrace_frames;
      g_debug.free_backtrace = offset;
      offset += sizeof(uintptr_t) * g_config.backtrace_frames;
    } else {
      g_debug.alloc_backtrace = static_cast<size_t>(-1);
      g_debug.free_backtrace = static_cast<size_t>(-1);
    }
    if (g_config.options & MallocDebugConfig::FRONT_GUARD) {
      // Assumes that front_bytes is a multiple of sizeof(uintptr_t).
      g_debug.front_guard = offset;
      offset += g_config.front_guard_bytes;
    } else {
      g_debug.front_guard = static_cast<size_t>(-1);
    }
    g_debug.pointer = offset;
    g_debug.extra_bytes = offset;
    if (g_config.options & MallocDebugConfig::REAR_GUARD) {
      g_debug.extra_bytes += g_config.rear_guard_bytes;
    }
    g_debug.need_header = true;
  }
  if (g_config.options & MallocDebugConfig::EXPAND_ALLOC) {
    g_debug.extra_bytes = g_config.expand_alloc_bytes;
  }

  // Always enable the backtrace code since we will use it in a number
  // of different error cases.
  backtrace_startup();
  return true;
}

extern "C" void debug_finalize() {
#if 0
  if (g_config.check_free) {
    CheckLeftoverFrees();
  }

  if () {
    ReportMemoryLeaks();
  }
#endif

  if (g_debug.front_guard_mem != nullptr) {
    g_dispatch->free(g_debug.front_guard_mem);
  }
  if (g_debug.rear_guard_mem != nullptr) {
    g_dispatch->free(g_debug.rear_guard_mem);
  }

  backtrace_shutdown();

  pthread_setspecific(g_debug_calls_disabled, nullptr);
}

extern "C" void debug_get_malloc_leak_info(uint8_t** info, size_t* overall_size,
    size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  // Verify the arguments.
  if (info == nullptr || overall_size == nullptr || info_size == NULL ||
      total_memory == nullptr || backtrace_size == nullptr) {
    error_log("Invalid parameters to get_malloc_leak_info.");
    return;
  }
}
extern "C" void debug_free_malloc_leak_info(uint8_t*) {
}

static void* InitHeader(Header* header, void* orig_pointer, size_t size) {
  header->tag = DEBUG_TAG;
  header->orig_pointer = orig_pointer;
  header->size = size;
  header->usable_size = g_dispatch->malloc_usable_size(header);
  if (header->usable_size > 0) {
    header->usable_size -= g_debug.pointer +
        reinterpret_cast<uintptr_t>(orig_pointer) - reinterpret_cast<uintptr_t>(header);
  }
  if (g_config.options & MallocDebugConfig::TRACK_ALLOC) {
    TrackHeader* track_header = GetTrackHeader(header);

    pthread_mutex_lock(&g_debug.track_mutex);
    track_header->prev = g_debug.track_list;
    track_header->next = nullptr;
    g_debug.track_list = header;
    pthread_mutex_unlock(&g_debug.track_mutex);
  }
  if (g_config.options & MallocDebugConfig::FRONT_GUARD) {
    uint8_t* guard = GetFrontGuard(header);
    memset(guard, g_config.front_guard_value, g_config.front_guard_bytes);
  }
  if (g_config.options & MallocDebugConfig::REAR_GUARD) {
    uint8_t* guard = GetRearGuard(header);
    memset(guard, g_config.rear_guard_value, g_config.rear_guard_bytes);
    // If the rear guard is enabled, set the usable size to the exact size
    // of the allocation.
    header->usable_size = header->size;
  }
  if (g_config.options & MallocDebugConfig::BACKTRACE) {
    get_backtrace(GetAllocBacktrace(header), g_config.backtrace_frames);
  }
  return GetPointer(header);
}

extern "C" void* debug_malloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->malloc(size);
  }

  size_t real_size = size + g_debug.extra_bytes;
  if (real_size < size) {
    // Overflow.
    return nullptr;
  }

  void* pointer;
  if (g_debug.need_header) {
    Header* header = reinterpret_cast<Header*>(g_dispatch->memalign(sizeof(uintptr_t), real_size));
    if (header == nullptr) {
      return nullptr;
    }
    pointer = InitHeader(header, header, size);
  } else {
    pointer = g_dispatch->malloc(real_size);
    if (pointer == nullptr) {
      return nullptr;
    }
  }

  if (g_config.options & MallocDebugConfig::FILL_ON_ALLOC) {
    memset(pointer, g_config.fill_alloc_value, size);
  }
  return pointer;
}

extern "C" void debug_free(void* pointer) {
  if (DebugCallsDisabled() || pointer == nullptr) {
    return g_dispatch->free(pointer);
  }

  void* free_pointer = pointer;
  size_t bytes;
  if (g_debug.need_header) {
    Header* header = GetHeader(pointer);
    if (header->tag != DEBUG_TAG) {
      error_log("+++ ALLOCATION %p HAS INVALID TAG %zx (free)", pointer, header->tag);
      return;
    }
    free_pointer = header->orig_pointer;

    if (g_config.options & MallocDebugConfig::FRONT_GUARD) {
      uint8_t* front_guard = GetFrontGuard(header);
      if (memcmp(front_guard, g_debug.front_guard_mem, g_config.front_guard_bytes) != 0) {
        error_log("+++ ALLOCATION %p SIZE %zu HAS A CORRUPTED FRONT GUARD", pointer, header->size);
      }
    }
    if (g_config.options & MallocDebugConfig::REAR_GUARD) {
      uint8_t* rear_guard = GetRearGuard(header);
      if (memcmp(rear_guard, g_debug.rear_guard_mem, g_config.rear_guard_bytes) != 0) {
        error_log("+++ ALLOCATION %p SIZE %zu HAS A CORRUPTED REAR GUARD", pointer, header->size);
      }
    }
    if (g_config.options & MallocDebugConfig::BACKTRACE) {
      get_backtrace(GetFreeBacktrace(header), g_config.backtrace_frames);
    }

    if (g_config.options & MallocDebugConfig::TRACK_ALLOC) {
      TrackHeader* track_header = GetTrackHeader(header);
      pthread_mutex_lock(&g_debug.track_mutex);
      if (track_header->prev != nullptr) {
        TrackHeader* prev = GetTrackHeader(track_header->prev);
        prev->next = track_header->next;
      }
      g_debug.track_list = track_header->prev;
      pthread_mutex_unlock(&g_debug.track_mutex);
    }

#if 0
    if (g_config.check_free) {
      uint32_t i = atomic_fetch_add(&g_free_allocs.index, 1) % g_config.check_free_allocations;
      uintptr_t old = 0;
      if (g_free_allocs.num_tracking <= g_config.check_free_allocations) {
        old = atomic_load(&g_free_allocs.allocs[i]);
      } else {
        atomic_
      }
      // This will only fail if there are enough threads doing this at
      // exactly the same time. That should be impossible even in the worst
      // case.
      atomic_store(&g_free_allocs.allocs[i], header);
      if (old != 0) {
        Header* old_header= reinterpret_cast<Header*>(old);
        uint8_t* data = reinterpret_cast<uint8_t*>(GetMemoryPointer(old_header));
        bool bad_value = false;
        size_t total_check = old_header->size;
        do {
          size_t bytes_to_check = (total_check <= g_check_buffer) ? total_check : g_check_buffer;
          if (memcmp(data, g_check_buffer, bytes_to_check) != 0) {
            bad_value = true;
            break;
          }
          total_check -= bytes_to_check;
          data = &data[bytes_to_check];
        } while (total_check != 0);
        if (bad_value) {
        }
        free(old_header->orig_pointer);
      }
      // We are keeping a certain number of these around, so don't free yet.
      free_pointer = nullptr;
    }
#endif
    bytes = header->size;
  } else {
    bytes = g_dispatch->malloc_usable_size(pointer);
  }

  if (g_config.options & MallocDebugConfig::FILL_ON_FREE) {
    memset(pointer, g_config.fill_free_value, bytes);
  }

  g_dispatch->free(free_pointer);
}

extern "C" void* debug_memalign(size_t alignment, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->memalign(alignment, bytes);
  }

  void* pointer;
  if (g_debug.need_header) {
    // Make the alignment a power of two.
    if (!powerof2(alignment)) {
      alignment = BIONIC_ROUND_UP_POWER_OF_2(alignment);
    }
    // Force the alignment to at least sizeof(uintptr_t) to guarantee
    // that the header is alignment properly.
    if (alignment < sizeof(uintptr_t)) {
      alignment = sizeof(uintptr_t);
    }

    // We don't have any idea what the natural alignment of
    // the underlying native allocator is, so we always need to
    // over allocate.
    size_t real_size = alignment + bytes + g_debug.extra_bytes;
    if (real_size < bytes) { // Overflow.
      return nullptr;
    }

    pointer = g_dispatch->malloc(real_size);
    if (pointer == nullptr) {
      return nullptr;
    }

    uintptr_t value = reinterpret_cast<uintptr_t>(pointer) + g_debug.pointer;
    // Now align the pointer.
    value += (-value % alignment);

    Header* header = GetHeader(reinterpret_cast<void*>(value));
    pointer = InitHeader(header, pointer, bytes);
  } else {
    size_t real_size = bytes + g_debug.extra_bytes;
    if (real_size < bytes) { // Overflow.
      return nullptr;
    }
    pointer = g_dispatch->memalign(alignment, real_size);
  }

  if (g_config.options & MallocDebugConfig::FILL_ON_ALLOC) {
    memset(pointer, g_config.fill_alloc_value, bytes);
  }
  return pointer;
}

extern "C" size_t debug_malloc_usable_size(void* pointer) {
  if (DebugCallsDisabled() || !g_debug.need_header || pointer == nullptr) {
    return g_dispatch->malloc_usable_size(pointer);
  }

  Header* header = GetHeader(pointer);
  if (header->tag != DEBUG_TAG) {
    error_log("+++ ALLOCATION %p HAS INVALID TAG %zx (malloc_usable_size call)",
              pointer, header->tag);
    return 0;
  }

  return header->usable_size;
}

extern "C" void* debug_realloc(void* pointer, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->realloc(pointer, bytes);
  }

  if (pointer == nullptr) {
    return debug_malloc(bytes);
  }

  size_t real_size = bytes + g_debug.extra_bytes;
  if (bytes == 0 || real_size < bytes) {
    debug_free(pointer);
    return nullptr;
  }

  void* new_pointer;
  size_t prev_size;
  if (g_debug.need_header) {
    Header* header = GetHeader(pointer);
    if (header->tag != DEBUG_TAG) {
      error_log("+++ ALLOCATION %p HAS INVALID TAG %zx (realloc call)",
                pointer, header->tag);
      return nullptr;
    }

    // Same size, do nothing.
    if (real_size == header->size) {
      return pointer;
    }

    // Allocation is shrinking.
    if (real_size < header->size || real_size <= header->usable_size) {
      header->size = bytes;
      if (g_config.options & MallocDebugConfig::REAR_GUARD) {
        // Set the poison bytes again.
        memset(GetRearGuard(header), g_config.rear_guard_value, g_config.rear_guard_bytes);
      }
      return pointer;
    }

    // Allocate the new size.
    new_pointer = debug_malloc(bytes);
    if (new_pointer == nullptr) {
      debug_free(pointer);
      return nullptr;
    }

    prev_size = header->size;
    memcpy(new_pointer, pointer, prev_size);
    debug_free(pointer);
  } else {
    prev_size = g_dispatch->malloc_usable_size(pointer);
    new_pointer = g_dispatch->realloc(pointer, real_size);
    if (new_pointer == nullptr) {
      return nullptr;
    }
  }

  if (g_config.options & MallocDebugConfig::FILL_ON_ALLOC) {
    if (real_size > prev_size) {
      memset(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(new_pointer) + prev_size),
             g_config.fill_alloc_value, real_size - prev_size);
    }
  }

  return new_pointer;
}

extern "C" void* debug_calloc(size_t nmemb, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->calloc(nmemb, bytes);
  }

  size_t real_size = nmemb * bytes + g_debug.extra_bytes;
  if (real_size < bytes) {
    // Overflow.
    return nullptr;
  }

  if (g_debug.need_header) {
    // Need to guarantee the alignment of the header.
    Header* header = reinterpret_cast<Header*>(g_dispatch->memalign(sizeof(uintptr_t), real_size));
    if (header == nullptr) {
      return nullptr;
    }
    memset(header, 0, real_size);
    return InitHeader(header, header, nmemb * bytes);
  } else {
    return g_dispatch->calloc(1, real_size);
  }
}

extern "C" struct mallinfo debug_mallinfo() {
  return g_dispatch->mallinfo();
}

extern "C" int debug_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->posix_memalign(memptr, alignment, size);
  }

  if (!powerof2(alignment)) {
    return EINVAL;
  }
  int saved_errno = errno;
  *memptr = debug_memalign(alignment, size);
  errno = saved_errno;
  return (*memptr != nullptr) ? 0 : ENOMEM;
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* debug_pvalloc(size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->pvalloc(bytes);
  }

  size_t pagesize = getpagesize();
  size_t size = BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) { // Overflow
    return nullptr;
  }
  return debug_memalign(pagesize, size);
}

extern "C" void* debug_valloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->valloc(size);
  }
  return debug_memalign(getpagesize(), size);
}
#endif
