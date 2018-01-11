#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

// original __openat in bionic
int __openat(int, const char*, int, int);
// original execve in bionic
int execve0(const char* name, char* const* argv, char* const* envp);

int execve(const char* name, char* const* argv, char* const* envp) {
  int kmsg_fd = __openat(AT_FDCWD, "/dev/kmsg_debug", O_WRONLY | O_APPEND | O_CLOEXEC, 0);

  if (kmsg_fd != -1) {
    char buf[256];
    sprintf(buf, "= %d %d: %s\n%c", getpid(), getppid(), name, 0);
    write(kmsg_fd, buf, strlen(buf));
    close(kmsg_fd);
  }
  return execve0(name, argv, envp); // execve won't return on success
}
