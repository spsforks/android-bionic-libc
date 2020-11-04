/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <sys/cdefs.h>

#if defined(__BIONIC__)

#include <gtest/gtest.h>

#include <android-base/macros.h>
#include <arm_acle.h>
#include <bionic/mte.h>
#include <setjmp.h>
#include <thread>
#include "utils.h"

#include "platform/bionic/malloc.h"

#undef NDEBUG
#include <assert.h>

#include <sys/prctl.h>

#include "platform/bionic/malloc.h"
#include "platform/bionic/mte.h"
#include "utils.h"

#include "SignalUtils.h"

#include <bionic/malloc_tagged_pointers.h>

__attribute__((no_sanitize("hwaddress")))
static void test_tag_mismatch() {
  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  p[0] = 1;
  int* mistagged_p ATTRIBUTE_UNUSED =
      reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(p.get()) + (1ULL << 56));
  {
    ScopedDisableMTE x;
    { ScopedDisableMTE y; }
#if defined(__aarch64__)
    volatile int load ATTRIBUTE_UNUSED = *mistagged_p;
#endif
  }
#if defined(__aarch64__)
  if (mte_supported()) {
    EXPECT_DEATH(
        {
          volatile int load ATTRIBUTE_UNUSED = *mistagged_p;
        },
        "");
  }
#endif
}

TEST(mte_test, ScopedDisableMTE) {
  // With native_bridge, native and emulated parts exchange data, including pointers.
  // This implies tagging on native and emulated architectures should match, which is
  // not the case at the moment.
  SKIP_WITH_NATIVE_BRIDGE;

  test_tag_mismatch();
}

#if defined(__aarch64__)
static void check_stack_can_be_tagged() {
  alignas(16) char buf[16];
  void* volatile p = &buf;
  EXPECT_EQ(p, __arm_mte_get_tag(p));
  void* q = __arm_mte_increment_tag(p, 1);
  __arm_mte_set_tag(q);
  EXPECT_EQ(q, __arm_mte_get_tag(q));
  __arm_mte_set_tag(p);
};
#endif

// Test that tagging is enabled on the default stacks (regular and signal).
TEST(mte_test, stack) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  struct sigaction sa = {};
  sa.sa_sigaction = [](int, siginfo_t* siginfo, void*) { check_stack_can_be_tagged(); };
  sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
  struct sigaction oldsa;
  ASSERT_EQ(0, sigaction(SIGUSR1, &sa, &oldsa));

  check_stack_can_be_tagged();  // main thread stack
  raise(SIGUSR1);               // main thread signal stack

  // same for a secondary thread
  std::thread t([]() {
    check_stack_can_be_tagged();
    raise(SIGUSR1);
  });
  t.join();

  // restore signal behavior
  ASSERT_EQ(0, sigaction(SIGUSR1, &oldsa, nullptr));
#endif
}

TEST(mte_test, prot_mte) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  // Without PROT_MTE, tag mismatch is ignored.
  char* p = static_cast<char*>(
      mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0));
  char* q = __arm_mte_increment_tag(p, 1);
  *p = 42;
  EXPECT_EQ(42, *q);
  // Allocation tags can not be changed.
  __arm_mte_set_tag(q);
  EXPECT_EQ(p, __arm_mte_get_tag(q));

  // With PROT_MTE, tags can be changed, and mismatch generates a fault.
  mprotect(p, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_MTE);
  __arm_mte_set_tag(q);
  EXPECT_EQ(q, __arm_mte_get_tag(q));
  EXPECT_EQ(42, *q);
  EXPECT_DEATH({ volatile int load ATTRIBUTE_UNUSED = *p; }, "");
  munmap(p, PAGE_SIZE);
#endif
}

#if defined(__aarch64__)
constexpr size_t kStackAllocationSize = 128 * 1024;

// Prevent optimizations.
volatile void* sink;

// Either execve or _exit, transferring control back to parent.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void vfork_child2(
    bool exec, void* fp_parent) {
  // Make sure that the buffer in the caller has not been optimized out.
  void* fp = __builtin_frame_address(0);
  assert(reinterpret_cast<uintptr_t>(fp_parent) - reinterpret_cast<uintptr_t>(fp) >=
         kStackAllocationSize);
  if (exec) {
    char* argv[] = {"/system/bin/true", nullptr};
    char* envp[] = {nullptr};
    execve("/system/bin/true", argv, envp);
    _exit(1);
  } else {
    _exit(0);
  }
}

// Place a tagged buffer on the stack. Do not tag the top half so that the parent does not crash too
// early even if things go wrong.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void vfork_child(bool exec) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = __arm_mte_increment_tag(p, 1);
    __arm_mte_set_tag(q);
    assert(__arm_mte_get_tag(p) == q);
  }
  vfork_child2(exec, __builtin_frame_address(0));
}

// Parent. Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void vfork_parent(pid_t pid) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    void* q = __arm_mte_get_tag(p);
    ASSERT_EQ(p, q);
  }

  int wstatus;
  do {
    pid_t w = waitpid(pid, &wstatus, 0);
  } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

  EXPECT_TRUE(WIFEXITED(wstatus));
  EXPECT_EQ(0, WEXITSTATUS(wstatus));
}
#endif  // defined(__aarch64__)

TEST(mte_test, vfork) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  pid_t pid = vfork();
  if (pid == 0) {
    vfork_child(false /* exec */);
  } else {
    vfork_parent(pid);
  }
#endif  // defined(__aarch64__)
}

TEST(mte_test, vfork_exec) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  pid_t pid = vfork();
  if (pid == 0) {
    vfork_child(true /* exec */);
  } else {
    vfork_parent(pid);
  }
#endif  // defined(__aarch64__)
}

#if defined(__aarch64__)
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void settag_and_longjmp(
    jmp_buf cont) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = __arm_mte_increment_tag(p, 1);
    __arm_mte_set_tag(q);
    assert(__arm_mte_get_tag(p) == q);
  }
  longjmp(cont, 42);
}

// Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void check_stack_tags() {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    void* q = __arm_mte_get_tag(p);
    if (p != q) fprintf(stderr, "stack tags mismatch");
    ASSERT_EQ(p, q) << "stack tags mismatch: expected " << p << ", got " << q;
  }
}

void check_longjmp_restores_tags() {
  int value;
  jmp_buf jb;
  if ((value = setjmp(jb)) == 0) {
    settag_and_longjmp(jb);
    FAIL();  // Unreachable.
  } else {
    ASSERT_EQ(42, value);
    check_stack_tags();
  }
}
#endif

TEST(mte_test, longjmp) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  check_longjmp_restores_tags();
#endif
}

TEST(mte_test, longjmp_sigaltstack) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  constexpr size_t kAltStackSize = kStackAllocationSize + PAGE_SIZE * 16;
  void* altstack_start = mmap(nullptr, kAltStackSize, PROT_READ | PROT_WRITE | PROT_MTE,
                              MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  stack_t ss = {};
  ss.ss_sp = altstack_start;
  ss.ss_size = kAltStackSize;
  stack_t old_ss;
  ASSERT_EQ(0, sigaltstack(&ss, &old_ss));

  struct sigaction sa = {};
  sa.sa_sigaction = [](int, siginfo_t* siginfo, void*) { check_longjmp_restores_tags(); };
  sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
  struct sigaction oldsa;
  ASSERT_EQ(0, sigaction(SIGUSR1, &sa, &oldsa));

  raise(SIGUSR1);  // main thread signal stack

  // same for a secondary thread
  std::thread t([]() {
    void* altstack_start = mmap(nullptr, kAltStackSize, PROT_READ | PROT_WRITE | PROT_MTE,
                                MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    stack_t ss = {};
    ss.ss_sp = altstack_start;
    ss.ss_size = kAltStackSize;
    stack_t old_ss;
    ASSERT_EQ(0, sigaltstack(&ss, &old_ss));

    raise(SIGUSR1);

    ASSERT_EQ(0, sigaltstack(&old_ss, nullptr));
    munmap(altstack_start, kAltStackSize);
  });
  t.join();

  // restore signal behavior
  ASSERT_EQ(0, sigaction(SIGUSR1, &oldsa, nullptr));
  ASSERT_EQ(0, sigaltstack(&old_ss, nullptr));
  munmap(altstack_start, kAltStackSize);
#endif
}

#if defined(__aarch64__)
static __thread jmp_buf g_jmp_buf;

__attribute__((no_sanitize("memtag"), optnone, noinline)) static void settag_and_raise() {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = __arm_mte_increment_tag(p, 1);
    __arm_mte_set_tag(q);
    assert(__arm_mte_get_tag(p) == q);
  }
  raise(SIGUSR1);
}
#endif

TEST(mte_test, longjmp_from_sigaltstack_unsupported) {
#if defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;
  if (!mte_supported()) {
    return;
  }

  constexpr size_t kAltStackSize = kStackAllocationSize + PAGE_SIZE * 16;
  void* altstack_start = mmap(nullptr, kAltStackSize, PROT_READ | PROT_WRITE | PROT_MTE,
                              MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  stack_t ss = {};
  ss.ss_sp = altstack_start;
  ss.ss_size = kAltStackSize;
  stack_t old_ss;
  ASSERT_EQ(0, sigaltstack(&ss, &old_ss));

  struct sigaction sa = {};
  sa.sa_sigaction = [](int, siginfo_t* siginfo, void*) { longjmp(g_jmp_buf, 42); };
  sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
  struct sigaction oldsa;
  ASSERT_EQ(0, sigaction(SIGUSR1, &sa, &oldsa));

  int value;
  if ((value = setjmp(g_jmp_buf)) == 0) {
    settag_and_raise();
    // raise(SIGUSR1);
    FAIL();  // Unreachable.
  } else {
    ASSERT_EQ(42, value);
    int tagged_addr_ctrl = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
    tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | PR_MTE_TCF_SYNC;
    prctl(PR_SET_TAGGED_ADDR_CTRL, tagged_addr_ctrl, 0, 0, 0) == 0;

    // Bionic does not reset stack tags when jumping from the signal stack to the regular stack, so
    // the following will fail.
    //    ASSERT_EQ(1, 0) << "zzzzzzzzz";
    check_stack_tags();
    // EXPECT_DEATH(check_stack_tags(), "stack tags mismatch");
  }

  // restore signal behavior
  ASSERT_EQ(0, sigaction(SIGUSR1, &oldsa, nullptr));
  ASSERT_EQ(0, sigaltstack(&old_ss, nullptr));
  munmap(altstack_start, kAltStackSize);
#endif
}

// #ifdef __aarch64__
// void handler(int signo, siginfo_t* siginfo, void* ucontext_raw) {
//   ucontext_t *ucontext = reinterpret_cast<ucontext_t*>(ucontext_raw);
//   uintptr_t sp = ucontext->uc_mcontext.sp;
//   fprintf(stderr, "signal sp: %zx\n", sp);
//   fprintf(stderr, "signal sp address: %p\n", &ucontext->uc_mcontext.sp);
// }
// #endif

// TEST(mte_test, longjmp_altstack) {
// #if defined(__aarch64__)
//   SKIP_WITH_NATIVE_BRIDGE;
//   if (!mte_supported()) {
//     return;
//   }

//   void* altstack_start = mmap(nullptr, PAGE_SIZE * 16, PROT_READ | PROT_WRITE | PROT_MTE,
//                               MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
//   stack_t ss = {};
//   ss.ss_sp = altstack_start;
//   ss.ss_size = PAGE_SIZE * 16;
//   stack_t old_ss;
//   ASSERT_EQ(0, sigaltstack(&ss, &old_ss));

//   struct sigaction sa = {};
//   sa.sa_sigaction = &handler;
//   sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
//   struct sigaction oldsa;
//   ASSERT_EQ(0, sigaction(SIGUSR1, &sa, &oldsa));

//   fprintf(stderr, "fp before signal: %p\n", __builtin_frame_address(0));
//   fprintf(stderr, "altstack: %p .. %p\n", altstack_start, (char *)altstack_start + PAGE_SIZE *
//   16); raise(SIGUSR1); // main thread signal stack

//   // // same for a secondary thread
//   // std::thread t([]() {
//   //   check_stack_can_be_tagged();
//   //   raise(SIGUSR1);
//   // });
//   // t.join();

//   // restore signal behavior
//   ASSERT_EQ(0, sigaction(SIGUSR1, &oldsa, nullptr));
//   ASSERT_EQ(0, sigaltstack(&old_ss, nullptr));
//   munmap(altstack_start, PAGE_SIZE * 16);
// #endif
// }

#endif  // __BIONIC__
