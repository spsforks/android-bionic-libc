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

#include "memtag.h"
#include "malloc_common.h"

#include <platform/bionic/malloc.h>
#include <platform/bionic/mte_kernel.h>

#include <sys/prctl.h>

extern "C" void scudo_malloc_disable_memory_tagging();

bool SetMemoryTagLevel(void* arg, size_t arg_size) {
  if (arg_size != sizeof(int)) {
    return false;
  }
#if defined(__aarch64__) && defined(ANDROID_EXPERIMENTAL_MTE)
  int tag_level = *reinterpret_cast<int*>(arg);
  int tagged_addr_ctrl = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
  if (tag_level == M_MEMORY_TAG_LEVEL_NONE) {
    tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | PR_MTE_TCF_NONE;
    if (prctl(PR_SET_TAGGED_ADDR_CTRL, tagged_addr_ctrl, 0, 0, 0) < 0) {
      error_log("memtag: prctl failed");
      return false;
    }
    scudo_malloc_disable_memory_tagging();
    info_log("memtag: tag level set to 0");
    return true;
  }
  if ((tagged_addr_ctrl & PR_MTE_TCF_MASK) == PR_MTE_TCF_NONE) {
    error_log("memtag: re-enabling tagging after it was disabled is not supported");
    return false;
  }
  switch (tag_level) {
    case M_MEMORY_TAG_LEVEL_ASYNC: {
      tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | PR_MTE_TCF_ASYNC;
      break;
    }
    case M_MEMORY_TAG_LEVEL_SYNC: {
      tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | PR_MTE_TCF_SYNC;
      break;
    }
    default:
      return false;
  }
  if (prctl(PR_SET_TAGGED_ADDR_CTRL, tagged_addr_ctrl, 0, 0, 0) < 0) {
    error_log("memtag: prctl failed");
    return false;
  }
  info_log("memtag: tag level set to %d", tag_level);
  return true;
#else
  (void)arg;
  (void)arg_size;
  error_log("memtag: memory tagging not supported");
  return false;
#endif
}
