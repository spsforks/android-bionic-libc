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

#include <cutils/trace.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/system_properties.h>

// Tracing class for bionic.  To begin a trace at a specified point:
//   android::BionicScopedTrace("Trace message");
// The trace will end when the contructor goes out of scope, or
// when the destructor is called.

#define WRITE_OFFSET          32
#define PROPERTY_VALUE_MAX    92

namespace android {
class BionicScopedTrace {
public:
  explicit BionicScopedTrace(const char* message) {
  char value[PROPERTY_VALUE_MAX];

  // Find out which tags have been enabled on the command line and set
  // the value of tags accordingly.
  if (tags == ATRACE_TAG_NOT_READY) {
    __system_property_get("debug.atrace.tags.enableflags", value);
    tags = strtoull(value, NULL, 0);
  }

  // Open the kernel trace marker
  atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY | O_CLOEXEC);

  if (atrace_marker_fd == -1) {
    fprintf(stderr, "Could not open kernel trace file\n");
    fflush(stderr);
    abort();
  }

  if ((ATRACE_TAG_BIONIC & tags) != 0) {
    // If bionic tracing has been enabled, then write the message to the
    // kernel trace_marker.
    int length = strlen(message);
    char buf[length + WRITE_OFFSET];
    size_t len;

    len = snprintf(buf, length + WRITE_OFFSET, "B|%d|%s", getpid(), message);
    ssize_t wbytes = write(atrace_marker_fd, buf, len);

    // Error while writing
    if ((size_t)wbytes != len) {
      abort();
    }
  }
}

inline ~BionicScopedTrace() {
  if ((ATRACE_TAG_BIONIC & tags) != 0) {
    char c = 'E';
    ssize_t wbytes = write(atrace_marker_fd, &c, 1);

    // Error while writing
    if ((size_t)wbytes != 1) {
      abort();
    }

    close(atrace_marker_fd);
  }
}

private:
  int atrace_marker_fd;
  static uint64_t tags;
};

uint64_t BionicScopedTrace::tags = ATRACE_TAG_NOT_READY;

};
