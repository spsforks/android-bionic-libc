/*
 * Copyright (C) 2016 The Android Open Source Project
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

/*
Parent				Child1			Child2
pipe()
set up child1 stack
clone
PR_SET_PTRACER child1
PR_SET_DUMPABLE on
jemalloc_prefork()
sem_post			sem_wait
				SIGSTOP?
				PTRACE_ATTACH all
				SIGCONT?
				collect thread info
sem_wait			sem_post
jemalloc_postfork_parent()	fork			je_iterate
				PTRACE_DETACH all	walk live
waitpid(child1)			exit
PR_SET_PTRACER off
PR_SET_DUMPABLE off
clean up child1 stack
close pipe[0]						close pipe[1]
listen on pipe						log to pipe
wait on pipe[1] closed					close pipe[0]
*/

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "private/bionic_macros.h"
#include "private/bionic_prctl.h"

#include "PtracerThread.h"
#include "log.h"

class Stack {
 public:
  Stack(size_t size) : size_(size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    page_size_ = sysconf(_SC_PAGE_SIZE);
    size_ += page_size_*2; // guard pages
    base_ = mmap(NULL, size_, prot, flags, -1, 0);
    if (base_ == MAP_FAILED) {
      base_ = NULL;
      size_ = 0;
      return;
    }
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, base_, size_, "libmemleak stack");
    mprotect(base_, page_size_, PROT_NONE);
    mprotect(top(), page_size_, PROT_NONE);
  };
  ~Stack() {
    munmap(base_, size_);
  };
  void* top() {
    return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(base_) + size_ - page_size_);
  };
 private:
  DISALLOW_COPY_AND_ASSIGN(Stack);

  void *base_;
  size_t size_;
  size_t page_size_;
};

PtracerThread::PtracerThread(const std::function<int()>& func) :
    child_pid_(0), old_dumpable_(0) {
  stack_ = std::make_unique<Stack>(8192);
  if (stack_->top() == nullptr) {
    log_fatal("failed to mmap child stack: %s", strerror(errno));
  }

  func_ = std::function<int()>{[&, func]() -> int {
    // In the child thread, lock and unlock the mutex to wait for the parent
    // to finish setting up for the child thread
    std::unique_lock<std::mutex> lk(m_);
    lk.unlock();
    _exit(func());
  }};
}

PtracerThread::~PtracerThread() {
  Kill();
  Join();
  ClearTracer();
  stack_ = nullptr;
}

bool PtracerThread::Start() {
  std::unique_lock<std::mutex> lk(m_);

  // Convert from void(*)(void*) to lambda with captures
  auto proxy = [](void *arg) -> int {
    return (*reinterpret_cast<std::function<int()>*>(arg))();
  };

  child_pid_ = clone(proxy, stack_->top(),
       CLONE_VM|CLONE_FS|CLONE_FILES/*|CLONE_UNTRACED*/,
       reinterpret_cast<void*>(&func_));
  if (child_pid_ < 0) {
    log_error("failed to clone child: %s", strerror(errno));
    return false;
  }

  SetTracer(child_pid_);

  lk.unlock();

  return true;
}

int PtracerThread::Join() {
  if (child_pid_ == -1) {
    return -1;
  }
  int status;
  int ret = TEMP_FAILURE_RETRY(waitpid(child_pid_, &status, __WALL));
  if (ret < 0) {
    log_error("waitpid %d failed: %s", child_pid_, strerror(errno));
    return -1;
  }

  child_pid_ = -1;

  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  } else if (WIFSIGNALED(status)) {
    return -WTERMSIG(status);
  } else {
    log_error("unexpected status %x", status);
    return -1;
  }
}

void PtracerThread::Kill() {
  if (child_pid_ == -1) {
    return;
  }

  syscall(SYS_tkill, child_pid_, SIGKILL);
}

void PtracerThread::SetTracer(pid_t tracer_pid) {
  old_dumpable_ = prctl(PR_GET_DUMPABLE);
  if (old_dumpable_ == 0) {
    prctl(PR_SET_DUMPABLE, 1);
  }
  prctl(PR_SET_PTRACER, tracer_pid);
}

void PtracerThread::ClearTracer() {
  prctl(PR_SET_PTRACER, 0);
  if (old_dumpable_ == 0) {
    prctl(PR_SET_DUMPABLE, 0);
  }
}
