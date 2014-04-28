/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include "CrashCatch.h"

#include <sys/ptrace.h>
#include <sys/user.h>

/*
 * Shared function that make child process for tracing.
 */
static void mockTraceChild(int *child_pid)
{
  int pid = fork();
  *child_pid = pid;
  ASSERT_FALSE( pid == -1 );

  if (pid == 0)
    { // Child
      reset_death_signals(); // Reset CRASH_CATCH()
      if (ptrace(PTRACE_TRACEME, 0, 0, 0) != 0)
	{
	  exit(3450);
	}
      else
	{
	  raise(SIGSTOP);
	  exit(0);
	}
    }
  else
    { // Parent
      int state;
      ASSERT_FALSE( waitpid(pid, &state, WUNTRACED) < 0 ); // Wait for TRACEME
      ASSERT_NE(3450, WEXITSTATUS(state));
    }
}

/*
 * This test checks that
 *   ptrace() works and able to get/set regs.
 */
TEST(sys_ptrace, regs)
{
  CRASH_CATCH();

  int pid;

  { SCOPED_TRACE("ptrace() trace_init");
    mockTraceChild(&pid);
  }

  { SCOPED_TRACE("ptrace() test_regs");
    struct user regs_user;

    ASSERT_FALSE( ptrace(PTRACE_GETREGS, pid, 0, &regs_user.regs) != 0 );
    ASSERT_FALSE( ptrace(PTRACE_SETREGS, pid, 0, &regs_user.regs) != 0 );

    // Tell the child to continue
    EXPECT_FALSE( ptrace(PTRACE_CONT, pid, 0, 0) != 0 );
  }
}

/*
 * This test checks that
 *   ptrace() works and able to get/set fpregs (floating-point).
 */
TEST(sys_ptrace, fpregs)
{
  CRASH_CATCH();

  int pid;

  { SCOPED_TRACE("ptrace() trace_init");
    mockTraceChild(&pid);
  }

  { SCOPED_TRACE("ptrace() test_fpregs");
    struct user regs_user;

    ASSERT_FALSE( ptrace(PTRACE_GETFPREGS, pid, 0, &regs_user.regs) != 0 );
    ASSERT_FALSE( ptrace(PTRACE_SETFPREGS, pid, 0, &regs_user.regs) != 0 );

    // Tell the child to continue
    EXPECT_FALSE( ptrace(PTRACE_CONT, pid, 0, 0) != 0 );
  }
}

/*
 * This test checks that
 *   ptrace() works and able to get/set signal info.
 */
TEST(sys_ptrace, siginfo)
{
  CRASH_CATCH();

  int pid;

  { SCOPED_TRACE("ptrace() trace_init");
    mockTraceChild(&pid);
  }

  { SCOPED_TRACE("ptrace() test_fpregs");
    struct user regs_user;

    ASSERT_FALSE( ptrace(PTRACE_GETSIGINFO, pid, 0, &regs_user.regs) != 0 );
    ASSERT_FALSE( ptrace(PTRACE_SETSIGINFO, pid, 0, &regs_user.regs) != 0 );

    // Tell the child to continue
    EXPECT_FALSE( ptrace(PTRACE_CONT, pid, 0, 0) != 0 );
  }
}

/*
 * This test checks that
 *   ptrace() works and able to kill a child.
 */
TEST(sys_ptrace, kill)
{
  CRASH_CATCH();

  int pid;

  { SCOPED_TRACE("ptrace() trace_init");
    mockTraceChild(&pid);
  }

  { SCOPED_TRACE("ptrace() test_fpregs");

    ASSERT_FALSE( ptrace(PTRACE_KILL, pid, 0, 0) != 0 );

    // Tell the child to continue, but there is nothing to continue
    EXPECT_TRUE( ptrace(PTRACE_CONT, pid, 0, 0) != 0 );
  }
}
