/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "linker_compressed_sleb128.h"

#include <stdlib.h>

#include <async_safe/log.h>

#include "linker_allocator.h"
#include "linker_debug.h"


// Use a 32kb buffer, but leave space for allocator overhead.
constexpr size_t kDeflateBufferSize = 32 * 1024 - sizeof(page_info);


compressed_sleb128_decoder::compressed_sleb128_decoder(const uint8_t* buffer, size_t count) {
  // Skip over decoded length.
  stream_.next_in = const_cast<uint8_t*>(buffer) + 4;
  stream_.avail_in = count - 4;
}

compressed_sleb128_decoder::~compressed_sleb128_decoder() {
  if (stream_.avail_out) {
    inflateEnd(&stream_);
    free(stream_.next_out);
  }
}

bool compressed_sleb128_decoder::initialize() {
  stream_.avail_out = kDeflateBufferSize;
  stream_.next_out = static_cast<uint8_t*>(malloc(kDeflateBufferSize));
  if (!stream_.avail_out)
    return false;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
  // deflateInit is actually a macro that uses an old-style cast.
  int err = inflateInit(&stream_);
#pragma clang diagnostic pop
  if (err != Z_OK) {
    PRINT("inflateInit failed %d\n", err);
    return false;
  }

  current_ = stream_.next_out;
  return inflate_next();
}

bool compressed_sleb128_decoder::inflate_next() {
  uint8_t* orig_next_out = stream_.next_out;
  stream_.avail_out = kDeflateBufferSize;
  int err = inflate(&stream_, Z_NO_FLUSH);
  if (err < 0) {
    PRINT("inflate failed %d\n", err);
    return false;
  }
  stream_.next_out = orig_next_out;
  current_ = orig_next_out;
  end_ = current_ + kDeflateBufferSize - stream_.avail_out;
  PRINT("Inflated %d more bytes", kDeflateBufferSize - stream_.avail_out);

  return true;
}

size_t compressed_sleb128_decoder::pop_front() {
  size_t value = 0;
  static const size_t size = CHAR_BIT * sizeof(value);

  size_t shift = 0;
  uint8_t byte;

  do {
    if (current_ >= end_) {
      inflate_next();
      if (current_ >= end_) {
        async_safe_fatal("compressed_sleb128_decoder ran out of bounds");
      }
    }

    byte = *current_++;
    value |= (static_cast<size_t>(byte & 127) << shift);
    shift += 7;
  } while (byte & 128);

  if (shift < size && (byte & 64)) {
    value |= -(static_cast<size_t>(1) << shift);
  }

  return value;
}
