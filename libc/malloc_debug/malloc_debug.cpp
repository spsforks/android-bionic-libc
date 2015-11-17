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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/param.h>
#include <unistd.h>

#include <vector>

#include <private/bionic_macros.h>
#include <private/bionic_malloc_dispatch.h>
#include <private/bionic_malloc_debug.h>
#include <private/ScopedPthreadMutexLocker.h>

#include "backtrace.h"
#include "MallocDebugConfig.h"
#include "malloc_debug.h"
#include "malloc_debug_disable.h"

struct DebugData {
  size_t track = 0;
  size_t alloc_backtrace = 0;
  size_t free_backtrace = 0;
  size_t front_guard = 0;
  size_t pointer = 0;
  size_t extra_bytes = 0;
  bool need_header = false;

  // Used as an easy comparison for front and rear guards.
  void* front_guard_mem = nullptr;
  void* rear_guard_mem = nullptr;

  // List of freed allocations to track.
  pthread_mutex_t free_track_mutex = PTHREAD_MUTEX_INITIALIZER;
  Header** free_track_array = nullptr;
  void* free_track_mem = nullptr;
  size_t free_track_idx = 0;

  // Only used for tracking allocations.
  pthread_mutex_t track_mutex = PTHREAD_MUTEX_INITIALIZER;
  Header* track_list = nullptr;
  size_t track_total_allocations = 0;
};

DebugData g_debug;
pthread_key_t g_debug_calls_disabled;
static const MallocDispatch* g_dispatch;

static MallocDebugConfig g_config;


// The header for allocations contains a number of optional
// pieces depending on what options were selected
// If a header is neede

void* get_pointer(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<void*>(value + g_debug.pointer);
}

Header* get_header(const void* pointer) {
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<Header*>(value - g_debug.pointer);
}

TrackHeader* get_track_header(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<TrackHeader*>(value + g_debug.track);
}

BacktraceHeader* get_alloc_backtrace(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<BacktraceHeader*>(value + g_debug.alloc_backtrace);
}

BacktraceHeader* get_free_backtrace(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<BacktraceHeader*>(value + g_debug.free_backtrace);
}

uint8_t* get_frond_guard(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(header);
  return reinterpret_cast<uint8_t*>(value + g_debug.front_guard);
}

uint8_t* get_rear_guard(const Header* header) {
  uintptr_t value = reinterpret_cast<uintptr_t>(get_pointer(header));
  return reinterpret_cast<uint8_t*>(value + header->size);
}

static void log_tag_error(const Header* header, const void* pointer, const char* name) {
  ScopedDisableDebugCalls disable;

  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p HAS INVALID TAG %zx (%s)", pointer, header->tag, name);
  error_log("Backtrace at time of failure:");
  std::vector<uintptr_t> frames(64);
  size_t frame_num = backtrace_get(frames.data(), frames.size());
  frames.resize(frame_num);
  backtrace_log(frames.data(), frames.size());
  error_log(LOG_DIVIDER);
}

static void log_guard_failure(const Header* header, const void* pointer, bool front_guard) {
  ScopedDisableDebugCalls disable;

  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p SIZE %zu HAS A CORRUPTED %s GUARD", pointer, header->size,
            front_guard ? "FRONT" : "REAR");
  // Log all of the failing bytes.
  const uint8_t* expected;
  const uint8_t* real;
  size_t guard_length;
  if (front_guard) {
    expected = reinterpret_cast<uint8_t*>(g_debug.front_guard_mem);
    real = get_frond_guard(header);
    guard_length = g_config.front_guard_bytes;
  } else {
    expected = reinterpret_cast<uint8_t*>(g_debug.rear_guard_mem);
    real = get_rear_guard(header);
    guard_length = g_config.rear_guard_bytes;
  }
  for (size_t i = 0; i < guard_length; i++) {
    if (real[i] != expected[i]) {
      int pointer_idx;
      if (front_guard) {
        pointer_idx = i - guard_length;
      } else {
        pointer_idx = header->size + i;
      }
      error_log("  pointer[%d] = 0x%02x (expected 0x%02x)", pointer_idx, real[i], expected[i]);
    }
  }

  error_log("Backtrace at time of failure:");
  std::vector<uintptr_t> frames(64);
  size_t frame_num = backtrace_get(frames.data(), frames.size());
  frames.resize(frame_num);
  backtrace_log(frames.data(), frames.size());
  error_log(LOG_DIVIDER);
}

static void log_free_error(Header* header) {
  ScopedDisableDebugCalls disable;

  uint8_t* pointer = reinterpret_cast<uint8_t*>(get_pointer(header));

  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p USED AFTER FREE", pointer);
  for (size_t i = 0; i < header->size; i++) {
    if (pointer[i] != g_config.fill_free_value) {
      error_log("  pointer[%d] = 0x%02x (expected 0x%02x)", i, pointer[i],
                g_config.fill_free_value);
    }
  }
  if (g_config.backtrace_enabled) {
    error_log("Backtrace at time of free:");
    BacktraceHeader* back_header = get_free_backtrace(header);
    backtrace_log(&back_header->frames[0], back_header->num_frames);
  }
  error_log(LOG_DIVIDER);
}

static void verify_and_free(Header* header) {
  uint8_t* pointer = reinterpret_cast<uint8_t*>(get_pointer(header));

  size_t bytes = header->size;
  while (bytes != 0) {
    size_t bytes_to_cmp = (bytes < FREE_TRACK_MEM_BUFFER_SIZE) ? bytes : FREE_TRACK_MEM_BUFFER_SIZE;
    if (memcmp(g_debug.free_track_mem, pointer, bytes_to_cmp) != 0) {
      log_free_error(header);
      break;
    }
    bytes -= bytes_to_cmp;
    pointer = &pointer[bytes_to_cmp];
  }
  g_dispatch->free(header->orig_pointer);
}

static void free_global_data() {
  g_dispatch->free(g_debug.front_guard_mem);
  g_dispatch->free(g_debug.rear_guard_mem);

  g_dispatch->free(g_debug.free_track_array);
  g_dispatch->free(g_debug.free_track_mem);
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

  g_debug.front_guard_mem = nullptr;
  g_debug.rear_guard_mem = nullptr;

  g_debug.free_track_array = nullptr;
  g_debug.free_track_mem = nullptr;

  if (g_config.options & FRONT_GUARD) {
    // Create a buffer for fast comparisons of the front guard.
    g_debug.front_guard_mem = g_dispatch->malloc(g_config.front_guard_bytes);
    if (g_debug.front_guard_mem == nullptr) {
      error_log("failed to allocate memory for front guard memory: %s", strerror(errno));
      free_global_data();
      return false;
    }
    memset(g_debug.front_guard_mem, g_config.front_guard_value, g_config.front_guard_bytes);
  }

  if (g_config.options & REAR_GUARD) {
    // Create a buffer for fast comparisons of the rear guard.
    g_debug.rear_guard_mem = g_dispatch->malloc(g_config.rear_guard_bytes);
    if (g_debug.rear_guard_mem == nullptr) {
      error_log("failed to allocate memory for rear guard memory: %s", strerror(errno));
      free_global_data();
      return false;
    }
    memset(g_debug.rear_guard_mem, g_config.rear_guard_value, g_config.rear_guard_bytes);
  }

  if (g_config.options & FREE_TRACK) {
    g_debug.free_track_array = reinterpret_cast<Header**>(g_dispatch->calloc(sizeof(Header*), g_config.free_track_allocations));
    if (g_debug.free_track_array == nullptr) {
      error_log("failed to allocate memory for free track data: %s", strerror(errno));
      free_global_data();
      return false;
    }

    // Create a buffer for quick comparisons of the freed data.
    g_debug.free_track_mem = g_dispatch->malloc(FREE_TRACK_MEM_BUFFER_SIZE);
    if (g_debug.free_track_mem == nullptr) {
      error_log("failed to allocate memory for free track comparison memory: %s", strerror(errno));
      free_global_data();
      return false;
    }
    memset(g_debug.free_track_mem, g_config.fill_free_value, FREE_TRACK_MEM_BUFFER_SIZE);

    g_debug.free_track_idx = 0;
  }

  // Check to see if the options that require a header are enabled.
  if ((g_config.options & ~(NO_HEADER_OPTIONS)) != 0) {
    // Initialize all of the static header offsets.
    size_t offset = BIONIC_ALIGN(sizeof(Header), sizeof(uintptr_t));
    if (g_config.options & TRACK_ALLOCS) {
      g_debug.track = offset;
      offset += BIONIC_ALIGN(sizeof(TrackHeader), sizeof(uintptr_t));
      g_debug.track_list = nullptr;
      g_debug.track_total_allocations = 0;
    } else {
      g_debug.track = static_cast<size_t>(-1);
    }
    if (g_config.options & BACKTRACE) {
      size_t hdr_len = sizeof(BacktraceHeader) + sizeof(uintptr_t) * g_config.backtrace_frames - 1;
      g_debug.alloc_backtrace = offset;
      offset += BIONIC_ALIGN(hdr_len, sizeof(uintptr_t));
      g_debug.free_backtrace = offset;
      offset += BIONIC_ALIGN(hdr_len, sizeof(uintptr_t));
    } else {
      g_debug.alloc_backtrace = static_cast<size_t>(-1);
      g_debug.free_backtrace = static_cast<size_t>(-1);
    }
    if (g_config.options & FRONT_GUARD) {
      // Assumes that front_bytes is a multiple of sizeof(uintptr_t).
      g_debug.front_guard = offset;
      offset += g_config.front_guard_bytes;
    } else {
      g_debug.front_guard = static_cast<size_t>(-1);
    }
    g_debug.pointer = offset;
    g_debug.extra_bytes = offset;
    if (g_config.options & REAR_GUARD) {
      g_debug.extra_bytes += g_config.rear_guard_bytes;
    }
    g_debug.need_header = true;
  }
  if (g_config.options & EXPAND_ALLOC) {
    g_debug.extra_bytes = g_config.expand_alloc_bytes;
  }

  // Always enable the backtrace code since we will use it in a number
  // of different error cases.
  backtrace_startup();

  return true;
}

extern "C" void debug_report_leaks(bool free_allocations) {
  if (!(g_config.options & LEAK_TRACK)) {
    return;
  }

  ScopedDisableDebugCalls disable;

  Header* header = g_debug.track_list;
  size_t track_count = 0;
  while (header) {
    error_log("+++ %s leaked block of size %zu at %p (leak %zu of %zu)", getprogname(),
              header->size, get_pointer(header), ++track_count, g_debug.track_total_allocations);
    if (g_config.backtrace_enabled) {
      error_log("Backtrace at time of allocation:");
      BacktraceHeader* back_header = get_alloc_backtrace(header);
      backtrace_log(&back_header->frames[0], back_header->num_frames);
    }

    TrackHeader* track_header = get_track_header(header);
    Header* old_header = header;
    header = track_header->next;
    if (free_allocations) {
      free(old_header->orig_pointer);
    }
  }
}

extern "C" void debug_finalize() {
  if (g_config.options & FREE_TRACK) {
    for (long i = 0; i < g_config.free_track_allocations; i++) {
      if (g_debug.free_track_array[i] != nullptr) {
        verify_and_free(g_debug.free_track_array[i]);
        g_debug.free_track_array[i] = nullptr;
      }
    }
  }

  debug_report_leaks(true);

  free_global_data();

  backtrace_shutdown();

  pthread_setspecific(g_debug_calls_disabled, nullptr);
}

extern "C" void debug_get_malloc_leak_info(uint8_t** info, size_t* overall_size,
    size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  ScopedDisableDebugCalls disable;

  // Verify the arguments.
  if (info == nullptr || overall_size == nullptr || info_size == NULL ||
      total_memory == nullptr || backtrace_size == nullptr) {
    error_log("get_malloc_leak_info: At least one invalid parameter.");
    return;
  }

  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;

  if (!(g_config.options & TRACK_ALLOCS)) {
    error_log("get_malloc_leak_info: Allocations not being tracked, to enable "
              "set the option 'backtrace'.");
    return;
  }

  ScopedPthreadMutexLocker(&g_debug.track_mutex);
  if (g_debug.track_total_allocations == 0) {
    return;
  }

  *backtrace_size = g_config.backtrace_frames;
  *info_size = sizeof(size_t) * 2 + sizeof(uintptr_t) * *backtrace_size;
  *info = reinterpret_cast<uint8_t*>(g_dispatch->calloc(*info_size, g_debug.track_total_allocations));
  if (*info == nullptr) {
    return;
  }
  *overall_size = *info_size * g_debug.track_total_allocations;

  Header* header = g_debug.track_list;
  uint8_t* data = *info;
  while (header) {
    BacktraceHeader* back_header = get_alloc_backtrace(header);

    memcpy(data, &header->size, sizeof(size_t));
    memcpy(&data[sizeof(size_t)], &back_header->num_frames, sizeof(size_t));
    memcpy(&data[2 * sizeof(size_t)], &back_header->frames[0],
           back_header->num_frames * sizeof(uintptr_t));

    *total_memory += header->size;

    TrackHeader* track_header = get_track_header(header);
    header = track_header->next;

    data += *info_size;
  }
}

extern "C" void debug_free_malloc_leak_info(uint8_t* info) {
  g_dispatch->free(info);
}

static void* init_header(Header* header, void* orig_pointer, size_t size) {
  header->tag = DEBUG_TAG;
  header->orig_pointer = orig_pointer;
  header->size = size;
  header->usable_size = g_dispatch->malloc_usable_size(header);
  if (header->usable_size > 0) {
    header->usable_size -= g_debug.pointer +
        reinterpret_cast<uintptr_t>(orig_pointer) - reinterpret_cast<uintptr_t>(header);
  }
  if (g_config.options & TRACK_ALLOCS) {
    TrackHeader* track_header = get_track_header(header);

    pthread_mutex_lock(&g_debug.track_mutex);
    track_header->next = g_debug.track_list;
    if (track_header->next) {
      TrackHeader* next = get_track_header(track_header->next);
      next->prev = header;
    }
    track_header->prev = nullptr;
    g_debug.track_list = header;
    g_debug.track_total_allocations++;
    pthread_mutex_unlock(&g_debug.track_mutex);
  }
  if (g_config.options & FRONT_GUARD) {
    uint8_t* guard = get_frond_guard(header);
    memset(guard, g_config.front_guard_value, g_config.front_guard_bytes);
  }
  if (g_config.options & REAR_GUARD) {
    uint8_t* guard = get_rear_guard(header);
    memset(guard, g_config.rear_guard_value, g_config.rear_guard_bytes);
    // If the rear guard is enabled, set the usable size to the exact size
    // of the allocation.
    header->usable_size = header->size;
  }
  if (g_config.backtrace_enabled) {
    BacktraceHeader* back_header = get_alloc_backtrace(header);
    back_header->num_frames = backtrace_get(&back_header->frames[0], g_config.backtrace_frames);
  }
  return get_pointer(header);
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
    pointer = init_header(header, header, size);
  } else {
    pointer = g_dispatch->malloc(real_size);
    if (pointer == nullptr) {
      return nullptr;
    }
  }

  if (g_config.options & FILL_ON_ALLOC) {
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
    Header* header = get_header(pointer);
    if (header->tag != DEBUG_TAG) {
      log_tag_error(header, pointer, "free");
      return;
    }
    free_pointer = header->orig_pointer;

    if (g_config.options & FRONT_GUARD) {
      uint8_t* front_guard = get_frond_guard(header);
      if (memcmp(front_guard, g_debug.front_guard_mem, g_config.front_guard_bytes) != 0) {
        log_guard_failure(header, pointer, true);
      }
    }
    if (g_config.options & REAR_GUARD) {
      uint8_t* rear_guard = get_rear_guard(header);
      if (memcmp(rear_guard, g_debug.rear_guard_mem, g_config.rear_guard_bytes) != 0) {
        log_guard_failure(header, pointer, false);
      }
    }
    if (g_config.backtrace_enabled) {
      BacktraceHeader* back = get_free_backtrace(header);
      back->num_frames = backtrace_get(&back->frames[0], g_config.backtrace_frames);
    }

    if (g_config.options & TRACK_ALLOCS) {
      TrackHeader* track_header = get_track_header(header);
      pthread_mutex_lock(&g_debug.track_mutex);
      if (track_header->next != nullptr) {
        TrackHeader* next = get_track_header(track_header->next);
        next->prev = track_header->prev;
      }
      if (track_header->prev != nullptr) {
        TrackHeader* prev = get_track_header(track_header->prev);
        prev->next = track_header->next;
      }
      if (g_debug.track_list == header) {
        g_debug.track_list = track_header->next;
      }
      g_debug.track_total_allocations--;
      pthread_mutex_unlock(&g_debug.track_mutex);
    }

    if (g_config.options & FREE_TRACK) {
      Header* free_header = nullptr;
      pthread_mutex_lock(&g_debug.free_track_mutex);
      free_header = g_debug.free_track_array[g_debug.free_track_idx];

      g_debug.free_track_array[g_debug.free_track_idx++] = header;
      if (g_debug.free_track_idx == static_cast<size_t>(g_config.free_track_allocations)) {
        g_debug.free_track_idx = 0;
      }
      pthread_mutex_unlock(&g_debug.free_track_mutex);

      if (free_header) {
        verify_and_free(free_header);
      }

      // Do not free this pointer just yet.
      free_pointer = nullptr;
    }

    bytes = header->size;
  } else {
    bytes = g_dispatch->malloc_usable_size(pointer);
  }

  if (g_config.options & FILL_ON_FREE) {
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

    Header* header = get_header(reinterpret_cast<void*>(value));
    pointer = init_header(header, pointer, bytes);
  } else {
    size_t real_size = bytes + g_debug.extra_bytes;
    if (real_size < bytes) { // Overflow.
      return nullptr;
    }
    pointer = g_dispatch->memalign(alignment, real_size);
  }

  if (g_config.options & FILL_ON_ALLOC) {
    memset(pointer, g_config.fill_alloc_value, bytes);
  }
  return pointer;
}

extern "C" size_t debug_malloc_usable_size(void* pointer) {
  if (DebugCallsDisabled() || !g_debug.need_header || pointer == nullptr) {
    return g_dispatch->malloc_usable_size(pointer);
  }

  Header* header = get_header(pointer);
  if (header->tag != DEBUG_TAG) {
    log_tag_error(header, pointer, "malloc_usable_size");
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
    Header* header = get_header(pointer);
    if (header->tag != DEBUG_TAG) {
      log_tag_error(header, pointer, "realloc");
      return nullptr;
    }

    // Same size, do nothing.
    if (real_size == header->size) {
      return pointer;
    }

    // Allocation is shrinking.
    if (real_size < header->size || real_size <= header->usable_size) {
      header->size = bytes;
      if (g_config.options & REAR_GUARD) {
        // Set the poison bytes again.
        memset(get_rear_guard(header), g_config.rear_guard_value, g_config.rear_guard_bytes);
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

  if (g_config.options & FILL_ON_ALLOC) {
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
    return init_header(header, header, nmemb * bytes);
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
