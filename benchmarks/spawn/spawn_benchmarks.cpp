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

#include "spawn_benchmark.h"

#include "File.h"
#include "NativeInfo.h"

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);

  for (int i = 1; i != argc; ++i) {
    const char* memory_replay_prefix = "--memory_replay=";
    if (strncmp(argv[i], memory_replay_prefix, strlen(memory_replay_prefix)) == 0) {
      AllocEntry* entries;
      size_t num_entries;
      GetUnwindInfo(argv[i] + strlen(memory_replay_prefix), &entries, &num_entries);
      ProcessDump(entries, num_entries, 512, /*free_all=*/false);
    } else {
      fprintf(stderr, "%s: error: unrecognized command-line flag: %s\n", argv[0],
              argv[i]);
    }
  }

  benchmark::RunSpecifiedBenchmarks();
}
