#include <assert.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>

#if defined(__aarch64__)

template <typename T>
static inline void mte_set_tag(T* p) {
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "stg %[Ptr], [%[Ptr]]\n"
      :
      : [Ptr] "r"(p)
      : "memory");
}

template <typename T>
static inline T* mte_get_tag(T* p) {
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "ldg %[Ptr], [%[Ptr]]\n"
      : [Ptr] "+r"(p)::"memory");
  return p;
}

template <typename T>
static inline T* mte_increment_tag(T* p) {
  T* res;
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "addg %[Res], %[Ptr], #0, #1\n"
      : [Res] "=r"(res)
      : [Ptr] "r"(p));
  return res;
}

constexpr size_t kStackAllocationSize = 128 * 1024;

// Prevent optimizations.
volatile void* sink;

// Either execve or _exit, transferring control back to parent.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_child2(bool exec,
                                                                            void* fp_parent) {
  // Make sure that the buffer in the caller has not been optimized out.
  void* fp = __builtin_frame_address(0);
  assert(reinterpret_cast<uintptr_t>(fp_parent) - reinterpret_cast<uintptr_t>(fp) >=
         kStackAllocationSize);
  if (exec) {
    const char* argv[] = {"/system/bin/true", nullptr};
    const char* envp[] = {nullptr};
    execve("/system/bin/true", const_cast<char**>(argv), const_cast<char**>(envp));
    _exit(1);
  } else {
    _exit(0);
  }
}

// Place a tagged buffer on the stack. Do not tag the top half so that the parent does not crash too
// early even if things go wrong.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_child(bool exec) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = mte_increment_tag(p);
    mte_set_tag(q);
    assert(mte_get_tag(p) == q);
  }
  vfork_child2(exec, __builtin_frame_address(0));
}

// Parent. Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_parent(pid_t pid) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  fprintf(stderr, "vfork_parent %p\n", &buf);
  bool success = true;
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    char* q = mte_get_tag(p);
    if (p != q) {
      fprintf(stderr, "tag mismatch at offset %zx: %p != %p\n", p - buf, p, q);
      success = false;
      break;
    }
  }

  int wstatus;
  do {
    waitpid(pid, &wstatus, 0);
  } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

  assert(WIFEXITED(wstatus));
  assert(WEXITSTATUS(wstatus) == 0);

  if (!success) exit(1);
}

void test_vfork(bool exec) {
  pid_t pid = vfork();
  if (pid == 0) {
    vfork_child(false /* exec */);
  } else {
    vfork_parent(pid);
  }
}

void test_vfork_exec() {
  test_vfork(true);
}

void test_vfork_exit() {
  test_vfork(false);
}

__attribute__((no_sanitize("memtag"), optnone, noinline)) static void settag_and_longjmp(
    jmp_buf cont) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = mte_increment_tag(p);
    mte_set_tag(q);
    if (mte_get_tag(p) != q) {
      fprintf(stderr, "failed to set allocation tags on stack: %p != %p\n", mte_get_tag(p), q);
      exit(1);
    }
  }
  longjmp(cont, 42);
}

// Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void check_stack_tags() {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    void* q = mte_get_tag(p);
    if (p != q) {
      fprintf(stderr, "stack tags mismatch: expected %p, got %p", p, q);
      exit(1);
    }
  }
}

void check_longjmp_restores_tags() {
  int value;
  jmp_buf jb;
  if ((value = setjmp(jb)) == 0) {
    settag_and_longjmp(jb);
    exit(2);  // Unreachable.
  } else {
    assert(value == 42);
    check_stack_tags();
  }
}

class SigAltStackScoped {
  stack_t old_ss;
  void* altstack_start;
  size_t altstack_size;

 public:
  SigAltStackScoped(size_t sz) : altstack_size(sz) {
    altstack_start = mmap(nullptr, altstack_size, PROT_READ | PROT_WRITE | PROT_MTE,
                          MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    stack_t ss = {};
    ss.ss_sp = altstack_start;
    ss.ss_size = altstack_size;
    int res = sigaltstack(&ss, &old_ss);
    assert(res == 0);
  }

  ~SigAltStackScoped() {
    int res = sigaltstack(&old_ss, nullptr);
    assert(res == 0);
    munmap(altstack_start, altstack_size);
  }
};

class SigActionScoped {
  int signo;
  struct sigaction oldsa;

 public:
  typedef void (*handler_t)(int, siginfo_t* siginfo, void*);

  SigActionScoped(int signo, handler_t handler) : signo(signo) {
    struct sigaction sa = {};
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    int res = sigaction(signo, &sa, &oldsa);
    assert(res == 0);
  }

  ~SigActionScoped() {
    int res = sigaction(signo, &oldsa, nullptr);
    assert(res == 0);
  }
};

void test_longjmp() {
  check_longjmp_restores_tags();

  std::thread t([]() { check_longjmp_restores_tags(); });
  t.join();
}

void test_longjmp_sigaltstack() {
  constexpr size_t kAltStackSize = kStackAllocationSize + PAGE_SIZE * 16;
  SigAltStackScoped sigAltStackScoped(kAltStackSize);
  SigActionScoped sigActionScoped(
      SIGUSR1, [](int, siginfo_t* siginfo, void*) { check_longjmp_restores_tags(); });
  raise(SIGUSR1);

  // same for a secondary thread
  std::thread t([]() {
    SigAltStackScoped sigAltStackScoped(kAltStackSize);
    raise(SIGUSR1);
  });
  t.join();
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("nothing to do\n");
    return 1;
  }

  if (strcmp(argv[1], "vfork_exec") == 0) {
    test_vfork_exec();
    return 0;
  }

  if (strcmp(argv[1], "vfork_exit") == 0) {
    test_vfork_exit();
    return 0;
  }

  if (strcmp(argv[1], "longjmp") == 0) {
    test_longjmp();
    return 0;
  }

  if (strcmp(argv[1], "longjmp_sigaltstack") == 0) {
    test_longjmp_sigaltstack();
    return 0;
  }

  printf("unrecognized command: %s\n", argv[1]);
  return 1;
}
#else
int main(int argc, char** argv) {
  printf("aarch64 only\n");
  return 1;
}
#endif  // defined(__aarch64__)
