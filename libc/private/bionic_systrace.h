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

/* Tracing class for bionic.  To begin a trace at a specified point:
 *   android::BionicScopedTrace("Trace message");
 * The trace will end when the contructor goes out of scope, or
 * when the destructor is called.
 */

#define WRITE_OFFSET          32
#define PROPERTY_VALUE_MAX    92

namespace android {
class BionicScopedTrace {
  public:
    BionicScopedTrace(const char* message) {
      char value[PROPERTY_VALUE_MAX];

      /* Find out which tags have been enabled on the command line and set
       * the value of tags accordingly.
       */
      if(tags == ATRACE_TAG_NOT_READY) {
        __system_property_get("debug.atrace.tags.enableflags", value);
        tags = strtoull(value, NULL, 0);
      }

      /* Open the kernel trace marker */
      atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);

      if(atrace_marker_fd == -1) {
        fprintf(stderr, "Could not open kernel trace file\n");
        fflush(stderr);
        abort();
      }
      else {
        if (ATRACE_TAG_BIONIC & tags) {
          /* If bionic tracing has been enabled, then write the message to the
           * kernel trace_marker.
           */
          char buf[strlen(message) + WRITE_OFFSET];
          size_t len;

          len = snprintf(buf, strlen(message) + WRITE_OFFSET, "B|%d|%s", getpid(), message);
          ssize_t wbytes = write(atrace_marker_fd, buf, len);

          /* Error while writing */
          if (wbytes < 0) {
            fprintf(stderr, "Error writing to trace file: %s\n", strerror(errno));
            fflush(stderr);
          }

          /* Not everything was written */
          else if ((size_t)wbytes != len) {
            fprintf(stderr, "Warning: only %d of %d bytes written\n", wbytes, len);
            fflush(stderr);
          }
        }
      }
    }

    inline ~BionicScopedTrace() {
      if (ATRACE_TAG_BIONIC && tags) {
        char c = 'E';
        ssize_t wbytes = write(atrace_marker_fd, &c, 1);

        /* Error while writing */
        if (wbytes < 0) {
          fprintf(stderr, "Error writing to trace file: %s\n", strerror(errno));
          fflush(stderr);
        }
        
        /* Not everything was written */
        else if ((size_t)wbytes != 1) {
          fprintf(stderr, "Warning: only %d of %d bytes written\n", wbytes, 1);
          fflush(stderr);
        }

        close(atrace_marker_fd);
      }
    }

  private:
    int atrace_marker_fd;
    uint64_t tags = ATRACE_TAG_NOT_READY;
};
};
