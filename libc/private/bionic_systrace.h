/*
 * Copyright (C) 2012 The Android Open Source Project
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
#include <stdio.h>
#include <stdlib.h>
#include <cutils/trace.h>
#include <sys/system_properties.h>

/* Tracing class for bionic.  To begin a trace at a specified point:
 *   android::BionicScopedTrace("Trace message");
 * The trace will end when the contructor goes out of scope, or
 * when the destructor is called.
 */

#define ATRACE_MESSAGE_LENGTH 1024
#define PROPERTY_VALUE_MAX    92

namespace android {
  class BionicScopedTrace {
    public:
      BionicScopedTrace(const char* name) {
        char value[PROPERTY_VALUE_MAX];
        char *endptr;

        /* Find out which tags have been enabled on the command line and set
         * the value of tags accordingly.
         */
        if(tags == ATRACE_TAG_LAST + 1) {
          __system_property_get("debug.atrace.tags.enableflags", value);
          tags = strtoull(value, &endptr, 0);
        }

        /* Open the kernel trace marker if it has not already been opened */
        //if(atrace_marker_fd == -1)
          atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);

        if(atrace_marker_fd == -1) {
          // TODO: do I want error messages here?
          fprintf(stderr, "Could not open kernel trace file\n");
          fflush(stderr);
          abort();
        }
        else {
          if (ATRACE_TAG_BIONIC & tags) {
            /* If bionic tracing has been enabled, then write the message to the
             * kernel trace_marker.
             */
            char buf[strlen(name)];
            size_t len;

            /* I'm trying to cut this down based on the size of the message, since we
             * will already know that, but I don't want to underestimate and I'm not 
             * sure magic numbers are a good idea.  9 = 2 for "B|" + 1 for "|" + 1 for
             * terminating \0 + 5 for getpid(), since the maxpid is 40,000 something.
             * All of which involves a really large number of uncertain assumptions.
             */
            len = snprintf(buf, strlen(name) + 9, "B|%d|%s", getpid(), name);
            ssize_t wbytes = write(atrace_marker_fd, buf, len);

            /* Not sure if it's necessary to check both conditions here. */
            // Error
            if (wbytes < 0) {
              fprintf(stderr, "Error writing to trace file: %s\n", strerror(errno));
              fflush(stderr);
            }
            // Not everything was written
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

          // Error
          if (wbytes < 0) {
            fprintf(stderr, "Error writing to trace file: %s\n", strerror(errno));
            fflush(stderr);
          }
          // Not everything was written
          else if ((size_t)wbytes != len) {
            fprintf(stderr, "Warning: only %d of %d bytes written\n", wbytes, len);
            fflush(stderr);
          }

          close(atrace_marker_fd);
        }
      }

    private:
      int atrace_marker_fd = ATRACE_TAG_LAST + 1;
      uint64_t tags = -1;
  };
};
