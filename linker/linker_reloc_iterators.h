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

#ifndef __LINKER_RELOC_ITERATORS_H
#define __LINKER_RELOC_ITERATORS_H

#include "linker.h"

#include <string.h>

const size_t RELOCATION_GROUPED_BY_INFO_FLAG = 1;
const size_t RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG = 2;
const size_t RELOCATION_GROUPED_BY_ADDEND_FLAG = 4;
const size_t RELOCATION_GROUP_HAS_ADDEND_FLAG = 8;

class plain_reloc_iterator {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  plain_reloc_iterator(rel_t* rel_array, size_t count)
      : begin_(rel_array), end_(begin_ + count), current_(begin_) {}

  bool has_next() {
    return current_ < end_;
  }

  rel_t* next() {
    return current_++;
  }
 private:
  rel_t* const begin_;
  rel_t* const end_;
  rel_t* current_;

  DISALLOW_COPY_AND_ASSIGN(plain_reloc_iterator);
};

template <typename decoder_t>
class packed_reloc_iterator {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  explicit packed_reloc_iterator(decoder_t&& decoder)
      : decoder_(decoder) {
    // initialize fields
    memset(&reloc_, 0, sizeof(reloc_));
    relocation_count_ = decoder_.pop_front();
    reloc_.r_offset = decoder_.pop_front();
    relocation_index_ = 0;
    relocation_group_index_ = 0;
    group_size_ = 0;
  }

  bool has_next() const {
    return relocation_index_ < relocation_count_;
  }

  rel_t* next() {
    if (relocation_group_index_ == group_size_) {
      if (!read_group_fields()) {
        // Iterator is inconsistent state; it should not be called again
        // but in case it is let's make sure has_next() returns false.
        relocation_index_ = relocation_count_ = 0;
        return nullptr;
      }
    }

    if (is_relocation_grouped_by_offset_delta()) {
      reloc_.r_offset += group_r_offset_delta_;
    } else {
      reloc_.r_offset += decoder_.pop_front();
    }

    if (!is_relocation_grouped_by_info()) {
      reloc_.r_info = decoder_.pop_front();
    }

#if defined(USE_RELA)
    if (is_relocation_group_has_addend() &&
        !is_relocation_grouped_by_addend()) {
      reloc_.r_addend += decoder_.pop_front();
    }
#endif

    relocation_index_++;
    relocation_group_index_++;

    return &reloc_;
  }
 private:
  bool read_group_fields() {
    group_size_ = decoder_.pop_front();
    group_flags_ = decoder_.pop_front();

    if (is_relocation_grouped_by_offset_delta()) {
      group_r_offset_delta_ = decoder_.pop_front();
    }

    if (is_relocation_grouped_by_info()) {
      reloc_.r_info = decoder_.pop_front();
    }

    if (is_relocation_group_has_addend() &&
        is_relocation_grouped_by_addend()) {
#if !defined(USE_RELA)
      // This platform does not support rela, and yet we have it encoded in android_rel section.
      DL_ERR("unexpected r_addend in android.rel section");
      return false;
#else
      reloc_.r_addend += decoder_.pop_front();
    } else if (!is_relocation_group_has_addend()) {
      reloc_.r_addend = 0;
#endif
    }

    relocation_group_index_ = 0;
    return true;
  }

  bool is_relocation_grouped_by_info() {
    return (group_flags_ & RELOCATION_GROUPED_BY_INFO_FLAG) != 0;
  }

  bool is_relocation_grouped_by_offset_delta() {
    return (group_flags_ & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) != 0;
  }

  bool is_relocation_grouped_by_addend() {
    return (group_flags_ & RELOCATION_GROUPED_BY_ADDEND_FLAG) != 0;
  }

  bool is_relocation_group_has_addend() {
    return (group_flags_ & RELOCATION_GROUP_HAS_ADDEND_FLAG) != 0;
  }

  decoder_t decoder_;
  size_t relocation_count_;
  size_t group_size_;
  size_t group_flags_;
  size_t group_r_offset_delta_;
  size_t relocation_index_;
  size_t relocation_group_index_;
  rel_t reloc_;
};

template <typename decoder_t>
class packed_reloc_iterator2 {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  packed_reloc_iterator2(decoder_t&& decoder, ElfW(Addr) load_bias)
      : decoder_(decoder), load_bias_(load_bias)
  {
    relocation_count_ = decoder_.pop_front();
  }

  bool has_next() const {
    return relocation_index_ < relocation_count_;
  }

  rel_t* next() {
    if (relocation_index_ == same_info_end_index_) {
      size_t encoded_offset = decoder_.pop_front();
      same_info_end_index_ += encoded_offset & 1;
#if defined(USE_RELA)
      has_addends_ = (encoded_offset & 2) != 0;
#endif
      reloc_.r_offset = encoded_offset & ~3;
      if (relocation_index_ == same_info_end_index_) {
        same_info_end_index_ += decoder_.pop_front();
      }
      reloc_.r_info = decoder_.pop_front();
    } else {
      if (relocation_index_ == same_delta_end_index_) {
        offset_delta_ = decoder_.pop_front();
        same_delta_end_index_ = relocation_index_ + (offset_delta_ & 3);
        if (same_delta_end_index_ == relocation_index_) {
          same_delta_end_index_ += decoder_.pop_front();
        }
      }

      reloc_.r_offset += offset_delta_;
    }

#if defined(USE_RELA)
    if (has_addends_) {
      reloc_.r_addend = decoder_.pop_front();
    } else if (ELFW(R_TYPE)(reloc_.r_info) == R_GENERIC_RELATIVE ||
               ELFW(R_TYPE)(reloc_.r_info) == R_GENERIC_IRELATIVE) {
      reloc_.r_addend = *reinterpret_cast<ElfW(Addr)*>(reloc_.r_offset + load_bias_);
    } else {
      reloc_.r_addend = 0;
    }
#endif
    relocation_index_ += 1;
    return &reloc_;
  }

 private:
  decoder_t decoder_;
  ElfW(Addr) load_bias_;
  size_t relocation_count_ = 0;
  size_t relocation_index_ = 0;
  size_t same_info_end_index_ = 0;
  size_t same_delta_end_index_ = 0;
  size_t offset_delta_ = 0;
  rel_t reloc_ = {};
#if defined(USE_RELA)
  bool has_addends_ = false;
#endif
};

#endif  // __LINKER_RELOC_ITERATORS_H
