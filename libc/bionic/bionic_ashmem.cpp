/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <linux/ashmem.h>

#include <private/bionic_ashmem.h>

static int __libc_ashmem_create_region(const char* name, size_t byte_count) {
  int fd = open("/dev/ashmem", O_RDWR);
  if (fd == -1) {
    return -1;
  }

  int rc = ioctl(fd, ASHMEM_SET_NAME, name);
  if (rc < 0) {
    close(fd);
    return rc;
  }

  rc = ioctl(fd, ASHMEM_SET_SIZE, byte_count);
  if (rc < 0) {
    close(fd);
    return rc;
  }

  return fd;
}

__LIBC_HIDDEN__ void* __libc_ashmem_mmap(const char* name, size_t byte_count) {
  int fd = __libc_ashmem_create_region(name, byte_count);
  if (fd == -1) {
    return MAP_FAILED;
  }
  void* result = mmap(NULL, byte_count, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  close(fd);
  return result;
}
