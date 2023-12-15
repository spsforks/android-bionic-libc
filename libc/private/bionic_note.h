/*
 * Copyright (C) 2023 The Android Open Source Project
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

#pragma once

#include <elf.h>
#include <string.h>
#include "platform/bionic/macros.h"

static inline bool __get_elf_note(const ElfW(Phdr) * phdr_start, size_t phdr_ct,
                                  const ElfW(Addr) load_bias, unsigned desired_type,
                                  const char* desired_name, const ElfW(Nhdr) * *note_out,
                                  const char** desc_out) {
  for (size_t i = 0; i < phdr_ct; ++i) {
    const ElfW(Phdr)* phdr = &phdr_start[i];
    if (phdr->p_type != PT_NOTE) {
      continue;
    }
    ElfW(Addr) p = load_bias + phdr->p_vaddr;
    ElfW(Addr) note_end = load_bias + phdr->p_vaddr + phdr->p_memsz;
    while (p + sizeof(ElfW(Nhdr)) <= note_end) {
      const ElfW(Nhdr)* note = reinterpret_cast<const ElfW(Nhdr)*>(p);
      p += sizeof(ElfW(Nhdr));
      const char* name = reinterpret_cast<const char*>(p);
      p += align_up(note->n_namesz, 4);
      const char* desc = reinterpret_cast<const char*>(p);
      p += align_up(note->n_descsz, 4);
      if (p > note_end) {
        break;
      }
      if (note->n_type != desired_type) {
        continue;
      }
      size_t desired_name_len = strlen(desired_name);
      if (note->n_namesz != desired_name_len + 1 ||
          strncmp(desired_name, name, desired_name_len) != 0) {
        break;
      }
      *note_out = note;
      *desc_out = desc;
      return true;
    }
  }
  return false;
}
