#include <dlfcn.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  fprintf(stderr, "loading fdtrack\n");
  void* libfdtrack = dlopen("libfdtrack.so", RTLD_NOW);
  if (!libfdtrack) {
    errx(1, "failed to dlopen libfdtrack.so: %s", dlerror());
  }

  int fd1 = open("/dev/null", O_RDONLY | O_CLOEXEC);
  int fd2 = open("/dev/null", O_RDONLY | O_CLOEXEC);
  int fd3 = open("/dev/null", O_RDONLY | O_CLOEXEC);
  close(fd2);

  auto fdtrack_dump = reinterpret_cast<void(*)(void)>(dlsym(libfdtrack, "fdtrack_dump"));
  if (!fdtrack_dump) {
    errx(1, "failed to dlsym fdtrack_dump");
  }

  fdtrack_dump();

  fprintf(stderr, "logcat should have two tracked fds: %d, %d\n", fd1, fd3);
  return 0;
}
