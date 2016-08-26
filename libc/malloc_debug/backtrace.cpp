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

#include <dlfcn.h>
#include <errno.h>
#include <inttypes.h>
#include <malloc.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unwind.h>

#include <atomic>

#include "backtrace.h"
#include "debug_log.h"
#include "MapData.h"

#if defined(__LP64__)
#define PAD_PTR "016" PRIxPTR
#else
#define PAD_PTR "08" PRIxPTR
#endif

typedef struct _Unwind_Context __unwind_context;

extern "C" char* __cxa_demangle(const char*, char*, size_t*, int*);

static MapData g_map_data;
static const MapEntry* g_current_code_map = nullptr;

static _Unwind_Reason_Code find_current_map(__unwind_context* context, void*) {
  uintptr_t ip = _Unwind_GetIP(context);

  if (ip == 0) {
    return _URC_END_OF_STACK;
  }
  g_current_code_map = g_map_data.find(ip);
  return _URC_END_OF_STACK;
}

static pthread_key_t g_jmpbuf_key;
static void (*g_old_segfault_handler)(int, siginfo_t*, void*);
static std::atomic_uint g_active_backtraces;
static struct sigaction g_segfault_action;

static void handle_segfault(int signal, siginfo_t* info, void* context) {
  void* jmpbuf = pthread_getspecific(g_jmpbuf_key);
  if (jmpbuf == nullptr) {
    // This is a crash on a thread not doing a backtrace.
    if (g_old_segfault_handler != nullptr) {
      g_old_segfault_handler(signal, info, context);
    }
  }
  siglongjmp(*reinterpret_cast<sigjmp_buf*>(jmpbuf), 1);
}

void backtrace_startup() {
  pthread_key_create(&g_jmpbuf_key, nullptr);
  memset(&g_segfault_action, 0, sizeof(g_segfault_action));
  g_segfault_action.sa_sigaction = handle_segfault;
  g_segfault_action.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
  sigemptyset(&g_segfault_action.sa_mask);

  _Unwind_Backtrace(find_current_map, nullptr);
}

void backtrace_shutdown() {
  pthread_key_delete(g_jmpbuf_key);
}

struct stack_crawl_state_t {
  uintptr_t* frames;
  size_t frame_count;
  size_t cur_frame = 0;

  stack_crawl_state_t(uintptr_t* frames, size_t frame_count)
      : frames(frames), frame_count(frame_count) {}
};

static _Unwind_Reason_Code trace_function(__unwind_context* context, void* arg) {
  stack_crawl_state_t* state = static_cast<stack_crawl_state_t*>(arg);

  uintptr_t ip = _Unwind_GetIP(context);

  // The instruction pointer is pointing at the instruction after the return
  // call on all architectures.
  // Modify the pc to point at the real function.
  if (ip != 0) {
#if defined(__arm__)
    // If the ip is suspiciously low, do nothing to avoid a segfault trying
    // to access this memory.
    if (ip >= 4096) {
      // Check bits [15:11] of the first halfword assuming the instruction
      // is 32 bits long. If the bits are any of these values, then our
      // assumption was correct:
      //  b11101
      //  b11110
      //  b11111
      // Otherwise, this is a 16 bit instruction.
      uint16_t value = (*reinterpret_cast<uint16_t*>(ip - 2)) >> 11;
      if (value == 0x1f || value == 0x1e || value == 0x1d) {
        ip -= 4;
      } else {
        ip -= 2;
      }
    }
#elif defined(__aarch64__)
    // All instructions are 4 bytes long, skip back one instruction.
    ip -= 4;
#elif defined(__i386__) || defined(__x86_64__)
    // It's difficult to decode exactly where the previous instruction is,
    // so subtract 1 to estimate where the instruction lives.
    ip--;
#endif
    // Do not record the frames that fall in our own shared library.
    if (g_current_code_map && (ip >= g_current_code_map->start) && ip < g_current_code_map->end) {
      return _URC_NO_REASON;
    }
  }

  state->frames[state->cur_frame++] = ip;
  return (state->cur_frame >= state->frame_count) ? _URC_END_OF_STACK : _URC_NO_REASON;
}

size_t backtrace_get(uintptr_t* frames, size_t frame_count) {
  struct sigaction old_action;

  sigaction(SIGSEGV, &g_segfault_action, &old_action);
  if (!(old_action.sa_flags & SA_SIGINFO) || old_action.sa_sigaction != handle_segfault) {
    g_old_segfault_handler = old_action.sa_sigaction;
  }
  g_active_backtraces.fetch_add(1, std::memory_order_relaxed);

  sigjmp_buf jmp_data;
  stack_crawl_state_t state(frames, frame_count);
  pthread_setspecific(g_jmpbuf_key, &jmp_data);
  bool segfault_occurred = (sigsetjmp(jmp_data, 1) != 0);
  if (!segfault_occurred) {
    _Unwind_Backtrace(trace_function, &state);
  } else {
    if (state.cur_frame == 0) {
      // Add one frame to indicate we crashed while running.
      state.frames[state.cur_frame++] = 0xdeaddead;
    }
    printf("+++ Crashed while trying to unwind.\n");
  }

  if (g_active_backtraces.fetch_sub(1, std::memory_order_release) == 1) {
    // Last backtrace is done, reset the handler.
    sigaction(SIGSEGV, &g_segfault_action, nullptr);
  }
  return state.cur_frame;
}
std::string backtrace_string(const uintptr_t* frames, size_t frame_count) {
  std::string str;

  for (size_t frame_num = 0; frame_num < frame_count; frame_num++) {
    uintptr_t offset = 0;
    const char* symbol = nullptr;

    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(frames[frame_num]), &info) != 0) {
      offset = reinterpret_cast<uintptr_t>(info.dli_saddr);
      symbol = info.dli_sname;
    }

    uintptr_t rel_pc = offset;
    const MapEntry* entry = g_map_data.find(frames[frame_num], &rel_pc);

    const char* soname = (entry != nullptr) ? entry->name.c_str() : info.dli_fname;
    if (soname == nullptr) {
      soname = "<unknown>";
    }
    char buf[1024];
    if (symbol != nullptr) {
      char* demangled_symbol = __cxa_demangle(symbol, nullptr, nullptr, nullptr);
      const char* best_name = (demangled_symbol != nullptr) ? demangled_symbol : symbol;

      __libc_format_buffer(buf, sizeof(buf),
          "          #%02zd  pc %" PAD_PTR "  %s (%s+%" PRIuPTR ")\n", frame_num,
          rel_pc, soname, best_name, frames[frame_num] - offset);
      free(demangled_symbol);
    } else {
      __libc_format_buffer(buf, sizeof(buf),
          "          #%02zd  pc %" PAD_PTR "  %s\n", frame_num, rel_pc, soname);
    }
    str += buf;
  }

  return str;
}

void backtrace_log(const uintptr_t* frames, size_t frame_count) {
  error_log_string(backtrace_string(frames, frame_count).c_str());
}
