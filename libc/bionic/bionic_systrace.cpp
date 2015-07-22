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
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "private/bionic_systrace.h"
#include "private/libc_logging.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#define WRITE_OFFSET   32

constexpr char SYSTRACE_PROPERTY_NAME[] = "debug.atrace.tags.enableflags";

static pthread_once_t bionic_systrace_once;
static pthread_once_t bionic_systrace_file_once;

static const prop_info* g_pinfo;
static atomic_uint g_serial;
static atomic_uint_least64_t g_tags;
static int g_trace_marker_fd;

static uint64_t read_tag_from_property(const prop_info* pinfo) {
  char value[PROP_VALUE_MAX];
  __system_property_read(pinfo, 0, value);
  return strtoull(value, NULL, 0);
}

static void bionic_systrace_init() {
  // If g_pinfo is null, this means that systrace hasn't been run and it's safe to
  // assume that no trace writing will need to take place.  However, to avoid running
  // this costly find check each time, we set it to a non-tracing value so that next
  // time, it will just check the serial to see if the value has been changed.
  // this function also deals with the bootup case, during which the call to property
  // set will fail if the property server hasn't yet started.
  g_pinfo = __system_property_find(SYSTRACE_PROPERTY_NAME);
  if (g_pinfo == NULL) {
    __system_property_set(SYSTRACE_PROPERTY_NAME, "0");
    g_pinfo = __system_property_find(SYSTRACE_PROPERTY_NAME);
  }
  if (g_pinfo != NULL) {
    atomic_init(&g_serial, __system_property_serial(g_pinfo));
    atomic_init(&g_tags, read_tag_from_property(g_pinfo));
  }
}

static bool should_trace() {
  pthread_once(&bionic_systrace_once, bionic_systrace_init);
  if (g_pinfo == NULL) {
    return false;
  }

  // Find out which tags have been enabled on the command line and set
  // the value of tags accordingly.  If the value of the property changes,
  // the serial will also change, so the costly system_property_read function
  // can be avoided by calling the much cheaper system_property_serial
  // first.  The values within pinfo may change, but its location is guaranteed
  // not to move.
  const uint32_t cur_serial = __system_property_serial(g_pinfo);
  if (cur_serial != atomic_load(&g_serial)) {
    atomic_store(&g_serial, cur_serial);
    atomic_store(&g_tags, read_tag_from_property(g_pinfo));
  }
  return ((atomic_load(&g_tags) & ATRACE_TAG_BIONIC) != 0);
}

static void bionic_systrace_file_init() {
  g_trace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_CLOEXEC | O_WRONLY);
}

static int get_trace_marker_fd() {
  pthread_once(&bionic_systrace_file_once, bionic_systrace_file_init);
  return g_trace_marker_fd;
}

ScopedTrace::ScopedTrace(const char* message) {
  if (!should_trace()) {
    return;
  }

  int trace_marker_fd = get_trace_marker_fd();
  if (trace_marker_fd == -1) {
    return;
  }

  // If bionic tracing has been enabled, then write the message to the
  // kernel trace_marker.
  int length = strlen(message);
  char buf[length + WRITE_OFFSET];
  size_t len = snprintf(buf, length + WRITE_OFFSET, "B|%d|%s", getpid(), message);

  // Tracing may stop just after checking property and before writing the message.
  // So the write is acceptable to fail. See b/20666100.
  TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
}

ScopedTrace::~ScopedTrace() {
  if (!should_trace()) {
    return;
  }

  int trace_marker_fd = get_trace_marker_fd();
  if (trace_marker_fd == -1) {
    return;
  }

  TEMP_FAILURE_RETRY(write(trace_marker_fd, "E", 1));
}
