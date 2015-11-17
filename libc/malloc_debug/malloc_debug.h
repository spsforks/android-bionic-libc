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

#ifndef MALLOC_DEBUG_H
#define MALLOC_DEBUG_H

#include <stdint.h>

#include <private/bionic_malloc_dispatch.h>

struct Header {
  uint32_t tag;
  void* orig_pointer;
  size_t size;
  size_t usable_size;
  size_t real_size() const { return size & ~(1U << 31); }
  void set_zygote() { size |= 1U << 31; }
  static size_t max_size() { return (1U << 31) - 1; }
} __attribute__((packed));

struct TrackHeader {
  Header* prev = nullptr;
  Header* next = nullptr;
} __attribute__((packed));

struct BacktraceHeader {
  size_t num_frames;
  uintptr_t frames[0];
} __attribute__((packed));

constexpr uint32_t DEBUG_TAG = 0x1ee7d00d;
constexpr char LOG_DIVIDER[] = "*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***";
constexpr size_t FREE_TRACK_MEM_BUFFER_SIZE = 4096;

extern const MallocDispatch* g_dispatch;

#endif // MALLOC_DEBUG_H
