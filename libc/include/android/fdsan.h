/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/*
 * Error checking for close(2).
 *
 * Mishandling of file descriptor ownership is a common source of errors that
 * can be extremely difficult to diagnose. Mistakes like the following can
 * result in seemingly 'impossible' failures showing up on other threads that
 * happened to try to open a file descriptor between the buggy code's close and
 * fclose:
 *
 *     int print(int fd) {
 *         int rc;
 *         char buf[128];
 *         while ((rc = read(fd, buf, sizeof(buf))) > 0) {
 *             printf("%.*s", rc);
 *         }
 *         close(fd);
 *     }
 *
 *     int bug() {
 *         FILE* f = fopen("foo", "r");
 *         print(fileno(f));
 *         fclose(f);
 *     }
 *
 * To make it easier to find this class of bugs, bionic provides a method to
 * require that file descriptors are closed by their owners. File descriptors
 * can be associated with tags with which they must be closed. This allows
 * objects that conceptually own an fd (FILE*, unique_fd, etc.) to use their
 * own address at the tag, to enforce that closure of the fd must come as a
 * result of their own destruction (fclose, ~unique_fd, etc.)
 *
 * By default, a file descriptor's tag is 0, and close(fd) is equivalent to
 * closing fd with the tag 0.
 */

/*
 * For improved diagnostics, the type of a file descriptors owner can be
 * encoded in the most significant byte of the owner tag. Values of 0 and 0xff
 * are ignored, which allows for raw pointers to be used as owner tags without
 * modification.
 */
enum android_fdsan_owner_type {
  ANDROID_FDSAN_OWNER_TYPE_GENERIC = 0,

  /* FILE* */
  ANDROID_FDSAN_OWNER_TYPE_FILE = 1,

  /* DIR* */
  ANDROID_FDSAN_OWNER_TYPE_DIR = 2,

  /* android::base::unique_fd */
  ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD = 3,

  /* java.io.FileInputStream */
  ANDROID_FDSAN_OWNER_TYPE_FILEINPUTSTREAM = 4,

  /* java.io.FileOutputStream */
  ANDROID_FDSAN_OWNER_TYPE_FILEOUTPUTSTREAM = 5,

  /* java.io.RandomAccessFile */
  ANDROID_FDSAN_OWNER_TYPE_RANDOMACCESSFILE = 6,

  /* android.os.ParcelFileDescriptor */
  ANDROID_FDSAN_OWNER_TYPE_PARCELFILEDESCRIPTOR = 7,
};

/*
 * Create an owner tag with the specified type and least significant 56 bits of tag.
 */
uint64_t android_fdsan_create_owner_tag(enum android_fdsan_owner_type type, uint64_t tag) __INTRODUCED_IN_FUTURE;

/*
 * Exchange a file descriptor's tag.
 *
 * Logs and aborts if the fd's tag does not match expected_tag.
 */
void android_fdsan_exchange_owner_tag(int fd, uint64_t expected_tag, uint64_t new_tag) __INTRODUCED_IN_FUTURE;

/*
 * Close a file descriptor with a tag, and resets the tag to 0.
 *
 * Logs and aborts if the tag is incorrect.
 */
int android_fdsan_close_with_tag(int fd, uint64_t tag) __INTRODUCED_IN_FUTURE;

/*
 * Enable/disable error checking and return the previous state.
 *
 * Error checking is disabled in the child of a fork, to maintain compatibility
 * with code that forks, blindly closes file descriptors, and then execs.
 *
 * In cases such as the zygote, where the child has no intention of calling
 * exec, call this function to reenable fdsan checks.
 *
 * This function is not thread-safe and does not synchronize with checks of the
 * value, and so should probably only be called in single-threaded contexts
 * (e.g. postfork).
 */
bool android_fdsan_set_enabled(bool enabled);

__END_DECLS
