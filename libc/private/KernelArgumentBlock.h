/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef KERNEL_ARGUMENT_BLOCK_H
#define KERNEL_ARGUMENT_BLOCK_H

#include <elf.h>
#include <link.h>
#include <stdint.h>
#include <string.h>
#include <sys/auxv.h>

#include "private/bionic_macros.h"

struct abort_msg_t;
struct libc_shared_globals;

// When the kernel starts the dynamic linker, it passes a pointer to a block
// of memory containing argc, the argv array, the environment variable array,
// and the array of ELF aux vectors. This class breaks that block up into its
// constituents for easy access.
class KernelArgumentBlock {
 public:
  explicit KernelArgumentBlock(void* raw_args) {
    init(raw_args);
  }

  // Similar to ::getauxval but doesn't require the libc global variables to be set up,
  // so it's safe to call this really early on.
  unsigned long getauxval(unsigned long type) {
    for (ElfW(auxv_t)* v = auxv; v->a_type != AT_NULL; ++v) {
      if (v->a_type == type) {
        return v->a_un.a_val;
      }
    }
    return 0;
  }

  // Remove initial arguments from the kernel argument block, shifting the
  // envp and auxv blocks down. The main block needs to remain at the same
  // address because its address is the initial stack pointer value on entry to
  // a program, and it's 16-byte aligned.
  void shift_args(size_t i) {
    size_t shift_bytes = i * sizeof(char*);
    memmove(raw_begin,
            raw_begin + shift_bytes,
            raw_end - raw_begin - shift_bytes);
    *reinterpret_cast<uintptr_t*>(raw_begin) = argc - i;
    init(raw_begin);
  }

  int argc;
  char** argv;
  char** envp;
  ElfW(auxv_t)* auxv;

  // Other data that we want to pass from the dynamic linker to libc.so.
  abort_msg_t** abort_message_ptr;
  libc_shared_globals* shared_globals;

 private:
  char* raw_begin;
  char* raw_end;

  void init(void* raw_args) {
    uintptr_t* args = reinterpret_cast<uintptr_t*>(raw_args);
    argc = static_cast<int>(*args);
    argv = reinterpret_cast<char**>(args + 1);
    envp = argv + argc + 1;

    // Skip over all environment variable definitions to find the aux vector.
    // The end of the environment block is marked by a NULL pointer.
    char** p = envp;
    while (*p != nullptr) {
      ++p;
    }
    ++p; // Skip the NULL itself.

    auxv = reinterpret_cast<ElfW(auxv_t)*>(p);

    ElfW(auxv_t)* final_aux_val = auxv;
    while (final_aux_val->a_type != AT_NULL) {
      ++final_aux_val;
    }

    raw_begin = static_cast<char*>(raw_args);
    raw_end = reinterpret_cast<char*>(final_aux_val + 1);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(KernelArgumentBlock);
};

#endif // KERNEL_ARGUMENT_BLOCK_H
