/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <android/api-level.h>
#include <elf.h>
#include <errno.h>
#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/mman.h>

#include "async_safe/log.h"
#include "heap_tagging.h"
#include "libc_init_common.h"
#include "platform/bionic/macros.h"
#include "platform/bionic/mte.h"
#include "platform/bionic/page.h"
#include "platform/bionic/reserved_signals.h"
#include "private/KernelArgumentBlock.h"
#include "private/bionic_asm.h"
#include "private/bionic_asm_note.h"
#include "private/bionic_call_ifunc_resolver.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "pthread_internal.h"
#include "sys/system_properties.h"
#include "sysprop_helpers.h"

#if __has_feature(hwaddress_sanitizer)
#include <sanitizer/hwasan_interface.h>
#endif

// Leave the variable uninitialized for the sake of the dynamic loader, which
// links in this file. The loader will initialize this variable before
// relocating itself.
#if defined(__i386__)
__LIBC_HIDDEN__ void* __libc_sysinfo;
#endif

extern "C" int __cxa_atexit(void (*)(void *), void *, void *);
extern "C" const char* __gnu_basename(const char* path);

static void call_array(init_func_t** list, int argc, char* argv[], char* envp[]) {
  // First element is -1, list is null-terminated
  while (*++list) {
    (*list)(argc, argv, envp);
  }
}

#if defined(__aarch64__) || defined(__x86_64__)
extern __LIBC_HIDDEN__ __attribute__((weak)) ElfW(Rela) __rela_iplt_start[], __rela_iplt_end[];

static void call_ifunc_resolvers() {
  if (__rela_iplt_start == nullptr || __rela_iplt_end == nullptr) {
    // These symbols are not emitted by gold. Gold has code to do so, but for
    // whatever reason it is not being run. In these cases ifuncs cannot be
    // resolved, so we do not support using ifuncs in static executables linked
    // with gold.
    //
    // Since they are weak, they will be non-null when linked with bfd/lld and
    // null when linked with gold.
    return;
  }

  for (ElfW(Rela) *r = __rela_iplt_start; r != __rela_iplt_end; ++r) {
    ElfW(Addr)* offset = reinterpret_cast<ElfW(Addr)*>(r->r_offset);
    ElfW(Addr) resolver = r->r_addend;
    *offset = __bionic_call_ifunc_resolver(resolver);
  }
}
#else
extern __LIBC_HIDDEN__ __attribute__((weak)) ElfW(Rel) __rel_iplt_start[], __rel_iplt_end[];

static void call_ifunc_resolvers() {
  if (__rel_iplt_start == nullptr || __rel_iplt_end == nullptr) {
    // These symbols are not emitted by gold. Gold has code to do so, but for
    // whatever reason it is not being run. In these cases ifuncs cannot be
    // resolved, so we do not support using ifuncs in static executables linked
    // with gold.
    //
    // Since they are weak, they will be non-null when linked with bfd/lld and
    // null when linked with gold.
    return;
  }

  for (ElfW(Rel) *r = __rel_iplt_start; r != __rel_iplt_end; ++r) {
    ElfW(Addr)* offset = reinterpret_cast<ElfW(Addr)*>(r->r_offset);
    ElfW(Addr) resolver = *offset;
    *offset = __bionic_call_ifunc_resolver(resolver);
  }
}
#endif

static void apply_gnu_relro() {
  ElfW(Phdr)* phdr_start = reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR));
  unsigned long int phdr_ct = getauxval(AT_PHNUM);

  for (ElfW(Phdr)* phdr = phdr_start; phdr < (phdr_start + phdr_ct); phdr++) {
    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    ElfW(Addr) seg_page_start = PAGE_START(phdr->p_vaddr);
    ElfW(Addr) seg_page_end = PAGE_END(phdr->p_vaddr + phdr->p_memsz);

    // Check return value here? What do we do if we fail?
    mprotect(reinterpret_cast<void*>(seg_page_start), seg_page_end - seg_page_start, PROT_READ);
  }
}

static void layout_static_tls(KernelArgumentBlock& args) {
  StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  layout.reserve_bionic_tls();

  const char* progname = args.argv[0];
  ElfW(Phdr)* phdr_start = reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR));
  size_t phdr_ct = getauxval(AT_PHNUM);

  static TlsModule mod;
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  if (__bionic_get_tls_segment(phdr_start, phdr_ct, 0, &mod.segment)) {
    if (!__bionic_check_tls_alignment(&mod.segment.alignment)) {
      async_safe_fatal("error: TLS segment alignment in \"%s\" is not a power of 2: %zu\n",
                       progname, mod.segment.alignment);
    }
    mod.static_offset = layout.reserve_exe_segment_and_tcb(&mod.segment, progname);
    mod.first_generation = kTlsGenerationFirst;

    modules.module_count = 1;
    modules.static_module_count = 1;
    modules.module_table = &mod;
  } else {
    layout.reserve_exe_segment_and_tcb(nullptr, progname);
  }
  // Enable the fast path in __tls_get_addr.
  __libc_tls_generation_copy = modules.generation;

  layout.finish_layout();
}

#ifdef __aarch64__
static bool __read_memtag_note(const ElfW(Nhdr)* note, const char* name, const char* desc,
                               unsigned* result) {
  if (note->n_type != NT_ANDROID_TYPE_MEMTAG) {
    return false;
  }
  if (note->n_namesz != 8 || strncmp(name, "Android", 8) != 0) {
    return false;
  }
  // Previously (in Android 12), if the note was != 4 bytes, we check-failed
  // here. Let's be more permissive to allow future expansion.
  if (note->n_descsz < 4) {
    async_safe_fatal("unrecognized android.memtag note: n_descsz = %d, expected >= 4",
                     note->n_descsz);
  }
  *result = *reinterpret_cast<const ElfW(Word)*>(desc);
  return true;
}

static unsigned __get_memtag_note(const ElfW(Phdr)* phdr_start, size_t phdr_ct,
                                  const ElfW(Addr) load_bias) {
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
      unsigned ret;
      if (__read_memtag_note(note, name, desc, &ret)) {
        return ret;
      }
    }
  }
  return 0;
}

// Returns true if there's an environment setting (either sysprop or env var)
// that should overwrite the ELF note, and places the equivalent heap tagging
// level into *level.
static bool get_environment_memtag_setting(HeapTaggingLevel* level) {
  static const char kMemtagPrognameSyspropPrefix[] = "arm64.memtag.process.";
  static const char kMemtagGlobalSysprop[] = "persist.arm64.memtag.default";
  static const char kMemtagOverrideSyspropPrefix[] =
      "persist.device_config.memory_safety_native.mode_override.process.";

  const char* progname = __libc_shared_globals()->init_progname;
  if (progname == nullptr) return false;

  const char* basename = __gnu_basename(progname);

  char options_str[PROP_VALUE_MAX];
  char sysprop_name[512];
  async_safe_format_buffer(sysprop_name, sizeof(sysprop_name), "%s%s", kMemtagPrognameSyspropPrefix,
                           basename);
  char remote_sysprop_name[512];
  async_safe_format_buffer(remote_sysprop_name, sizeof(remote_sysprop_name), "%s%s",
                           kMemtagOverrideSyspropPrefix, basename);
  const char* sys_prop_names[] = {sysprop_name, remote_sysprop_name, kMemtagGlobalSysprop};

  if (!get_config_from_env_or_sysprops("MEMTAG_OPTIONS", sys_prop_names, arraysize(sys_prop_names),
                                       options_str, sizeof(options_str))) {
    return false;
  }

  if (strcmp("sync", options_str) == 0) {
    *level = M_HEAP_TAGGING_LEVEL_SYNC;
  } else if (strcmp("async", options_str) == 0) {
    *level = M_HEAP_TAGGING_LEVEL_ASYNC;
  } else if (strcmp("off", options_str) == 0) {
    *level = M_HEAP_TAGGING_LEVEL_TBI;
  } else {
    async_safe_format_log(
        ANDROID_LOG_ERROR, "libc",
        "unrecognized memtag level: \"%s\" (options are \"sync\", \"async\", or \"off\").",
        options_str);
    return false;
  }

  return true;
}

// Returns the initial heap tagging level. Note: This function will never return
// M_HEAP_TAGGING_LEVEL_NONE, if MTE isn't enabled for this process we enable
// M_HEAP_TAGGING_LEVEL_TBI.
static HeapTaggingLevel __get_heap_tagging_level(const void* phdr_start, size_t phdr_ct,
                                                 uintptr_t load_bias, bool* stack) {
  unsigned note_val =
      __get_memtag_note(reinterpret_cast<const ElfW(Phdr)*>(phdr_start), phdr_ct, load_bias);
  *stack = note_val & NT_MEMTAG_STACK;

  HeapTaggingLevel level;
  if (get_environment_memtag_setting(&level)) return level;

  // Note, previously (in Android 12), any value outside of bits [0..3] resulted
  // in a check-fail. In order to be permissive of further extensions, we
  // relaxed this restriction.
  if (!(note_val & (NT_MEMTAG_HEAP | NT_MEMTAG_STACK))) return M_HEAP_TAGGING_LEVEL_TBI;

  unsigned mode = note_val & NT_MEMTAG_LEVEL_MASK;
  switch (mode) {
    case NT_MEMTAG_LEVEL_NONE:
      // Note, previously (in Android 12), NT_MEMTAG_LEVEL_NONE was
      // NT_MEMTAG_LEVEL_DEFAULT, which implied SYNC mode. This was never used
      // by anyone, but we note it (heh) here for posterity, in case the zero
      // level becomes meaningful, and binaries with this note can be executed
      // on Android 12 devices.
      return M_HEAP_TAGGING_LEVEL_TBI;
    case NT_MEMTAG_LEVEL_ASYNC:
      return M_HEAP_TAGGING_LEVEL_ASYNC;
    case NT_MEMTAG_LEVEL_SYNC:
    default:
      // We allow future extensions to specify mode 3 (currently unused), with
      // the idea that it might be used for ASYMM mode or something else. On
      // this version of Android, it falls back to SYNC mode.
      return M_HEAP_TAGGING_LEVEL_SYNC;
  }
}

// Figure out the desired memory tagging mode (sync/async, heap/globals/stack) for this executable.
// This function is called from the linker before the main executable is relocated.
__attribute__((no_sanitize("hwaddress", "memtag"))) void __libc_init_mte(const void* phdr_start,
                                                                         size_t phdr_ct,
                                                                         uintptr_t load_bias,
                                                                         void* stack_top) {
  bool memtag_stack;
  HeapTaggingLevel level = __get_heap_tagging_level(phdr_start, phdr_ct, load_bias, &memtag_stack);
  char* env = getenv("BIONIC_MEMTAG_UPGRADE_SECS");
  static const char kAppProcessNamePrefix[] = "app_process";
  const char* progname = __libc_shared_globals()->init_progname;
  if (progname &&
      strncmp(progname, kAppProcessNamePrefix, sizeof(kAppProcessNamePrefix) - 1) == 0) {
    // disable timed upgrade for zygote, as the thread spawned will violate the requirement
    // that it be single-threaded.
    env = nullptr;
  }
  int64_t timed_upgrade = 0;
  if (env) {
    char* endptr;
    timed_upgrade = strtoll(env, &endptr, 10);
    if (*endptr != '\0' || timed_upgrade < 0) {
      async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                            "Invalid value for BIONIC_MEMTAG_UPGRADE_SECS: %s",
                            env);
      timed_upgrade = 0;
    }
    // Make sure that this does not get passed to potential processes inheriting
    // this environment.
    unsetenv("BIONIC_MEMTAG_UPGRADE_SECS");
  }
  if (timed_upgrade) {
    if (level == M_HEAP_TAGGING_LEVEL_ASYNC) {
      async_safe_format_log(ANDROID_LOG_INFO, "libc",
                            "Attempting timed MTE upgrade from async to sync.");
      __libc_shared_globals()->heap_tagging_upgrade_timer_sec = timed_upgrade;
      level = M_HEAP_TAGGING_LEVEL_SYNC;
    } else if (level != M_HEAP_TAGGING_LEVEL_SYNC) {
      async_safe_format_log(
          ANDROID_LOG_ERROR, "libc",
          "Requested timed MTE upgrade from invalid %s to sync. Ignoring.",
          DescribeTaggingLevel(level));
    }
  }
  if (level == M_HEAP_TAGGING_LEVEL_SYNC || level == M_HEAP_TAGGING_LEVEL_ASYNC) {
    unsigned long prctl_arg = PR_TAGGED_ADDR_ENABLE | PR_MTE_TAG_SET_NONZERO;
    prctl_arg |= (level == M_HEAP_TAGGING_LEVEL_SYNC) ? PR_MTE_TCF_SYNC : PR_MTE_TCF_ASYNC;

    // When entering ASYNC mode, specify that we want to allow upgrading to SYNC by OR'ing in the
    // SYNC flag. But if the kernel doesn't support specifying multiple TCF modes, fall back to
    // specifying a single mode.
    if (prctl(PR_SET_TAGGED_ADDR_CTRL, prctl_arg | PR_MTE_TCF_SYNC, 0, 0, 0) == 0 ||
        prctl(PR_SET_TAGGED_ADDR_CTRL, prctl_arg, 0, 0, 0) == 0) {
      __libc_shared_globals()->initial_heap_tagging_level = level;
      __libc_shared_globals()->initial_memtag_stack = memtag_stack;

      if (memtag_stack) {
        void* page_start =
            reinterpret_cast<void*>(PAGE_START(reinterpret_cast<uintptr_t>(stack_top)));
        if (mprotect(page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_MTE | PROT_GROWSDOWN)) {
          async_safe_fatal("error: failed to set PROT_MTE on main thread stack: %s\n",
                           strerror(errno));
        }
      }

      return;
    }
  }

  // MTE was either not enabled, or wasn't supported on this device. Try and use
  // TBI.
  if (prctl(PR_SET_TAGGED_ADDR_CTRL, PR_TAGGED_ADDR_ENABLE, 0, 0, 0) == 0) {
    __libc_shared_globals()->initial_heap_tagging_level = M_HEAP_TAGGING_LEVEL_TBI;
  }
  // We did not enable MTE, so we do not need to arm the upgrade timer.
  __libc_shared_globals()->heap_tagging_upgrade_timer_sec = 0;
}
#else   // __aarch64__
void __libc_init_mte(const void*, size_t, uintptr_t, void*) {}
#endif  // __aarch64__

void __libc_init_profiling_handlers() {
  // The dynamic variant of this function is more interesting, but this
  // at least ensures that static binaries aren't killed by the kernel's
  // default disposition for these two real-time signals that would have
  // handlers installed if this was a dynamic binary.
  signal(BIONIC_SIGNAL_PROFILER, SIG_IGN);
  signal(BIONIC_SIGNAL_ART_PROFILER, SIG_IGN);
}

__attribute__((no_sanitize("memtag"))) __noreturn static void __real_libc_init(
    void* raw_args, void (*onexit)(void) __unused, int (*slingshot)(int, char**, char**),
    structors_array_t const* const structors, bionic_tcb* temp_tcb) {
  BIONIC_STOP_UNWIND;

  // Initialize TLS early so system calls and errno work.
  KernelArgumentBlock args(raw_args);
  __libc_init_main_thread_early(args, temp_tcb);
  __libc_init_main_thread_late();
  __libc_init_globals();
  __libc_shared_globals()->init_progname = args.argv[0];
  __libc_init_AT_SECURE(args.envp);
  layout_static_tls(args);
  __libc_init_main_thread_final();
  __libc_init_common();
  __libc_init_mte(reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR)), getauxval(AT_PHNUM),
                  /*load_bias = */ 0, /*stack_top = */ raw_args);
  __libc_init_scudo();
  __libc_init_profiling_handlers();
  __libc_init_fork_handler();

  call_ifunc_resolvers();
  apply_gnu_relro();

  // Several Linux ABIs don't pass the onexit pointer, and the ones that
  // do never use it.  Therefore, we ignore it.

  call_array(structors->preinit_array, args.argc, args.argv, args.envp);
  call_array(structors->init_array, args.argc, args.argv, args.envp);

  // The executable may have its own destructors listed in its .fini_array
  // so we need to ensure that these are called when the program exits
  // normally.
  if (structors->fini_array != nullptr) {
    __cxa_atexit(__libc_fini,structors->fini_array,nullptr);
  }

  __libc_init_mte_late();

  exit(slingshot(args.argc, args.argv, args.envp));
}

extern "C" void __hwasan_init_static();

// This __libc_init() is only used for static executables, and is called from crtbegin.c.
//
// The 'structors' parameter contains pointers to various initializer
// arrays that must be run before the program's 'main' routine is launched.
__attribute__((no_sanitize("hwaddress", "memtag"))) __noreturn void __libc_init(
    void* raw_args, void (*onexit)(void) __unused, int (*slingshot)(int, char**, char**),
    structors_array_t const* const structors) {
  bionic_tcb temp_tcb = {};
#if __has_feature(hwaddress_sanitizer)
  // Install main thread TLS early. It will be initialized later in __libc_init_main_thread. For now
  // all we need is access to TLS_SLOT_SANITIZER.
  __set_tls(&temp_tcb.tls_slot(0));
  // Initialize HWASan enough to run instrumented code. This sets up TLS_SLOT_SANITIZER, among other
  // things.
  __hwasan_init_static();
  // We are ready to run HWASan-instrumented code, proceed with libc initialization...
#endif
  __real_libc_init(raw_args, onexit, slingshot, structors, &temp_tcb);
}

static int g_target_sdk_version{__ANDROID_API__};

extern "C" int android_get_application_target_sdk_version() {
  return g_target_sdk_version;
}

extern "C" void android_set_application_target_sdk_version(int target) {
  g_target_sdk_version = target;
  __libc_set_target_sdk_version(target);
}

// This function is called in the dynamic linker before ifunc resolvers have run, so this file is
// compiled with -ffreestanding to avoid implicit string.h function calls. (It shouldn't strictly
// be necessary, though.)
__LIBC_HIDDEN__ libc_shared_globals* __libc_shared_globals() {
  static libc_shared_globals globals;
  return &globals;
}
