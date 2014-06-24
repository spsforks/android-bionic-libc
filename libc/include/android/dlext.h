/*
 * Copyright (C) 2014 The Android Open Source Project

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

#ifndef __ANDROID_DLEXT_H__
#define __ANDROID_DLEXT_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>  /* for off64_t */

__BEGIN_DECLS

/* bitfield definitions for android_dlextinfo.flags */
enum {
  /* When set, the reserved_addr and reserved_size fields must point to an
   * already-reserved region of address space which will be used to load the
   * library if it fits. If the reserved region is not large enough, the load
   * will fail.
   */
  ANDROID_DLEXT_RESERVED_ADDRESS      = 0x1,

  /* As DLEXT_RESERVED_ADDRESS, but if the reserved region is not large enough,
   * the linker will choose an available address instead.
   */
  ANDROID_DLEXT_RESERVED_ADDRESS_HINT = 0x2,

  /* When set, write the GNU RELRO section of the mapped library to relro_fd
   * after relocation has been performed, to allow it to be reused by another
   * process loading the same library at the same address. This implies
   * ANDROID_DLEXT_USE_RELRO.
   */
  ANDROID_DLEXT_WRITE_RELRO           = 0x4,

  /* When set, compare the GNU RELRO section of the mapped library to relro_fd
   * after relocation has been performed, and replace any relocated pages that
   * are identical with a version mapped from the file.
   */
  ANDROID_DLEXT_USE_RELRO             = 0x8,

  /* Instruct dlopen to use library_fd instead of opening file by name.
   * The filename parameter is still used to identify the library.
   */
  ANDROID_DLEXT_USE_LIBRARY_FD        = 0x10,

  /* If opening a library using library_fd read it starting at library_fd_offset.
   * This flag is only valid when ANDROID_DLEXT_USE_LIBRARY_FD is set.
   */

  ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET    = 0x20,

  /* When set, the lookup_fn field is used to open this library and all dependent
   * libraries.
   *
   * Note that if the ANDROID_DLEXT_USE_LIBRARY_FD flag is set the linker uses
   * the lookup function only for dt_needed libraries; the root library is going
   * to be linked using library_fd.
   */
  ANDROID_DLEXT_USE_LOOKUP_FUNCTION   = 0x40,

  /* Mask of flags preserved while loading dt_needed libraries.
   */
  ANDROID_DLEXT_RECURSIVE_FLAG_BITS   = ANDROID_DLEXT_USE_LOOKUP_FUNCTION,

  /* Mask of valid bits */
  ANDROID_DLEXT_VALID_FLAG_BITS       = ANDROID_DLEXT_RESERVED_ADDRESS |
                                        ANDROID_DLEXT_RESERVED_ADDRESS_HINT |
                                        ANDROID_DLEXT_WRITE_RELRO |
                                        ANDROID_DLEXT_USE_RELRO |
                                        ANDROID_DLEXT_USE_LIBRARY_FD |
                                        ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET |
                                        ANDROID_DLEXT_USE_LOOKUP_FUNCTION,
};

/* Opens the library named by 'filename' and sets file descriptor and
 * file offset in case of success. fd and offset are used by the linker to
 * load the library. The offset must be page-aligned.
 *
 * Returns true if file was successfully opened and false otherwise.
 *
 * If *close_flag is true (default) linker closes the fd once the library is
 * loaded or, in the case of an error, before return from android_dlopen_ext.
 */
typedef bool (*android_dlopen_lookup_fn_t)(const char* filename, int* fd, off64_t* offset, bool* close_flag, void* arg);

typedef struct {
  android_dlopen_lookup_fn_t lookup_fn;
  void* lookup_fn_arg;
} android_dlopen_lookup_action;

typedef struct {
  uint64_t flags;
  void*   reserved_addr;
  size_t  reserved_size;
  int     relro_fd;
  int     library_fd;
  off64_t library_fd_offset;
  android_dlopen_lookup_action library_lookup_action;
} android_dlextinfo;

void* android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo);

/* Sets global file lookup action for dlopen.
 */
void android_set_dlopen_lookup_action(const android_dlopen_lookup_action* new_action, android_dlopen_lookup_action* old_action);

__END_DECLS

#endif /* __ANDROID_DLEXT_H__ */
