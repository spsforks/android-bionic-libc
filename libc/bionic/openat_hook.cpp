#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static FILE *init(const char *log_path) {
  return fopen(log_path, "a");
}

extern "C" int __openat(int, const char*, int, int);

extern "C" int __openat_hook(int dir_fd, const char *path, int flag, int mode) {
//  if (strcmp(path, "/dev/kmsg_debug") == 0)
//    return __openat(dir_fd, path, flag, mode);

//  static int kmsg_fd = __openat(AT_FDCWD, "/dev/kmsg_debug", O_WRONLY | O_APPEND | O_CLOEXEC, 0);

  int fd = __openat(dir_fd, path, flag, mode);

  if (fd >= 0) {  // Let's only log every successful open!
    int kmsg_fd = __openat(AT_FDCWD, "/dev/kmsg_debug", O_WRONLY | O_APPEND | O_CLOEXEC, 0);
    struct stat s;
    if (fstat(fd, &s) == 0 && s.st_dev == 0xfd01) { // system partition
      char buf[256];
//      sprintf(buf, "_ %d: 0x%llx %lld, %s\n%c", getpid(), s.st_dev, s.st_ino, path, 0);
      sprintf(buf, "_ %d: 0x%llx %lld\n%c", getpid(), s.st_dev, s.st_ino, 0);
      write(kmsg_fd, buf, strlen(buf));
    }
    close(kmsg_fd);
  }
  return fd;
}

extern "C" int __openat_hook_2(int dir_fd, const char *path, int flag, int mode) {
  // TODO: init log_fd
  static FILE *log_fp = init("/dev/kmsg_debug");

  int fd = __openat(dir_fd, path, flag, mode);

  if (fd >= 0) {  // Let's log every successful open!
    struct stat s;
    if (fstat(fd, &s) == 0) {
      fprintf(log_fp, "_ %d: 0x%llx 0x%llx\n", getpid(), s.st_dev, s.st_ino);
    }
  }
  return fd;
}
#if 0
//#include <stdlib.h>
#include <string.h>

int __openat_hook_1(int fd, const char *path, int flag, int mode) {
  int log_it = 1;

  if (*path == '/')
    if (strcmp(path, "/system/") != 0) log_it = 0;

  int ret = __openat(fd, path, flag, mode);

  if (log_it != 0) {
    int pid = getpid();
    fprintf(log_fp, "%d: %s\n", pid, path);
  }

  return ret;
}
#endif
