/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <gtest/gtest.h>
#include <private/bionic_asm.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef __aarch64__

// This test verifies that the clone() system call preserves all general
// purpose register values except X0 and SP.
TEST(sys_clone, arm64_register_values) {
  long child;

  asm volatile(
      // Even though all general purpose registers are clobbered, we can't
      // put them all in the clobber list because the function fails to
      // compile, so we have to save and restore some of them manually.
      // 4 registers seems to be enough breathing room (and SP must be a
      // multiple of 16 anyway).
      "sub sp, sp, #32 \n\t"
      "stp x27, x28, [sp] \n\t"
      "stp x29, x30, [sp, #16] \n\t"
      "mov x0, %[clone_flags] \n\t"
      "mov x1, xzr \n\t"
      "mov x8, %[clone_syscall_nr] \n\t"
      ".irp reg, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,"
               " 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 \n\t"
      // Use a pseudorandom number for the reference value.
      "mov x\\reg, #((257 * \\reg + 131) %% 4096) \n\t"
      ".endr \n\t"
      "svc #0 \n\t"
      "cbnz x0, .Lparent \n\t"
      "mov x0, #1 \n\t"
      "cmp x1, xzr \n\t"
      "b.ne .Lchild_exit \n\t"
      "mov x0, #8 \n\t"
      "cmp x8, %[clone_syscall_nr] \n\t"
      "b.ne .Lchild_exit \n\t"
      ".irp reg, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,"
               " 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 \n\t"
      "mov x0, #\\reg \n\t"
      "cmp x\\reg, #((257 * \\reg + 131) %% 4096) \n\t"
      "b.ne .Lchild_exit \n\t"
      ".endr \n\t"
      "mov x0, #0 \n"
      ".Lchild_exit: \n\t"
      "mov x8, %[exit_syscall_nr] \n\t"
      "svc #0 \n\t"
      // Abort in case the system call fails for any reason.
      "brk #0 \n"
      ".Lparent: \n\t"
      "ldp x27, x28, [sp] \n\t"
      "ldp x29, x30, [sp, #16] \n\t"
      "add sp, sp, #32 \n\t"
      "mov %[result], x0 \n\t"
      : [result] "=r" (child)
      : [clone_flags] "i" (CLONE_VFORK),  // Not necessary, but makes use of some actual flags.
        [clone_syscall_nr] "i" (SYS_clone), [exit_syscall_nr] "i" (SYS_exit)
      : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13",
        "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26");
  ASSERT_NE(child, 0);

  // This is consistent with how syscall return values are checked by the rest of bionic.
  const int clone_errno = (child >= -static_cast<long>(MAX_ERRNO) && child < 0) ? -child : 0;

  ASSERT_EQ(clone_errno, 0);

  int status;
  // __WALL makes the logic work for any clone() flags.
  const pid_t waited_pid = waitpid(child, &status, __WALL);

  ASSERT_EQ(waited_pid, child);
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(WEXITSTATUS(status), 0);
}

#endif  // __aarch64__
