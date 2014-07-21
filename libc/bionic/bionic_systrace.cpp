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

#include "private/bionic_systrace.h"

#define WRITE_OFFSET   32

static prop_info* pinfo = const_cast<prop_info*>(__system_property_find("debug.atrace.tags.enableflags"));
static uint32_t serial = 0;
static uint64_t tags = ATRACE_TAG_NOT_READY;
static int trace_marker_fd = -1;

ScopedTrace::ScopedTrace(const char* message) {
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

  if ((tags & ATRACE_TAG_BIONIC) != 0) {
    if (trace_marker_fd == -1) {
      trace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY | O_CLOEXEC);
      if (trace_marker_fd == -1) {
        fprintf(stderr, "Could not open kernel trace file: %s\n", strerror(errno));
        fflush(stderr);
        abort();
      }
    }

    // If bionic tracing has been enabled, then write the message to the
    // kernel trace_marker.
    int length = strlen(message);
    char buf[length + WRITE_OFFSET];
    size_t len = snprintf(buf, length + WRITE_OFFSET, "B|%d|%s", getpid(), message);
    ssize_t wbytes = TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));

    // Error while writing
    if ((size_t)wbytes != len) {
      fprintf(stderr, "Could not write to kernel trace file: %s\n", strerror(errno));
      fflush(stderr);
      abort();
    }
  }
}

ScopedTrace::~ScopedTrace() {
  uint32_t cur_serial = __system_property_serial(pinfo);
  if (cur_serial != serial) {
    serial = cur_serial;
    char value[PROP_VALUE_MAX];
    __system_property_read(pinfo, 0, value);
    tags = strtoull(value, NULL, 0);
  }

  if ((tags & ATRACE_TAG_BIONIC) != 0) {
    ssize_t wbytes = TEMP_FAILURE_RETRY(write(trace_marker_fd, "E", 1));

    // Error while writing
    if ((size_t)wbytes != 1) {
      fprintf(stderr, "Could not write to kernel trace file: %s\n", strerror(errno));
      fflush(stderr);
      abort();
    }
  }
};