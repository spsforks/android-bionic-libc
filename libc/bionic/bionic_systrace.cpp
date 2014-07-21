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

#include "private/bionic_systrace.h"

// Tracing class for bionic.  To begin a trace at a specified point:
//   android::BionicScopedTrace("Trace message");
// The trace will end when the contructor goes out of scope, or
// when the destructor is called.

using namespace android;

BionicScopedTrace::BionicScopedTrace(const char* message) {
  // Find out which tags have been enabled on the command line and set
  // the value of tags accordingly.  If the value of the property changes,
  // the serial will also change, so the costly system_property_read function
  // can be avoided by calling the much cheaper system_property_serial
  // first.  The values within pinfo may change, but its location is guaranteed
  // not to move.
  uint32_t cur_serial = __system_property_serial(pinfo);
  if (cur_serial != serial) {
    serial = cur_serial;
    char value[PROP_VALUE_MAX];
    __system_property_read(pinfo, 0, value);
    tags = strtoull(value, NULL, 0);
  }

  if (atrace_marker_fd == -1) {
    fprintf(stderr, "Could not open kernel trace file: %s\n", strerror(errno));
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
      fprintf(stderr, "Could not write to kernel trace file: %s\n", strerror(errno));
      fflush(stderr);
      abort();
    }
  }
}

BionicScopedTrace::~BionicScopedTrace() {
  if ((ATRACE_TAG_BIONIC & tags) != 0) {
    char c = 'E';
    ssize_t wbytes = write(atrace_marker_fd, &c, 1);

    // Error while writing
    if ((size_t)wbytes != 1) {
      fprintf(stderr, "Could not write to kernel trace file: %s\n", strerror(errno));
      fflush(stderr);
      abort();
    }
  }
};

uint64_t BionicScopedTrace::tags = ATRACE_TAG_NOT_READY;
prop_info* BionicScopedTrace::pinfo = const_cast<prop_info*>(__system_property_find("debug.atrace.tags.enableflags"));
uint32_t BionicScopedTrace::serial = 0;
int BionicScopedTrace::atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY | O_CLOEXEC);