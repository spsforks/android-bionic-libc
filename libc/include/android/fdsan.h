/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef __ANDROID_FDSAN_H__
#define __ANDROID_FDSAN_H__

#include <stdbool.h>
#include <sys/cdefs.h>

#include <android/versioning.h>

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
 * Close a file descriptor with a tag, and resets the tag to 0.
 *
 * Logs and aborts if the tag is incorrect.
 */
int __libc_close_with_tag(int fd, void* tag) __INTRODUCED_IN_FUTURE;

/*
 * Exchange a file descriptor's tag.
 *
 * Logs and aborts if the fd's tag does not match expected_tag.
 */
void __libc_exchange_close_tag(int fd, void* expected_tag, void* new_tag) __INTRODUCED_IN_FUTURE;

/*
 * Enable/disable close error checking, returning the previous value.
 */
bool __libc_set_fdsan_enabled(bool enabled);

__END_DECLS

#endif
