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

namespace android
{
  class BionicScopedTrace
  {
    public:
      inline BionicScopedTrace(const char* name)
      {
        char value[PROPERTY_VALUE_MAX];
        char *endptr;

        /* Find out which tags have been enabled on the command line and set
         * the value of tags accordingly.
         */
        __system_property_get("debug.atrace.tags.enableflags", value);
        tags = strtoull(value, &endptr, 0);

        /* Open the kernel trace marker if it has not already been opened */
        if(atrace_marker_fd == -1)
          atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);

        if(atrace_marker_fd == -1)
        {
          // TODO: do I want error messages here?
          fprintf(stderr, "Could not open kernel trace file\n");
          fflush(stderr);
          abort();
        }
        else
        {
          if (bionic_enable & tags)
          {
            /* If bionic tracing has been enabled, then write the message to the
             * kernel trace_marker.
             */
            char buf[ATRACE_MESSAGE_LENGTH];
            size_t len;

            len = snprintf(buf, ATRACE_MESSAGE_LENGTH, "B|%d|%s", getpid(), name);
            write(atrace_marker_fd, buf, len);
          }
        }
      }

      inline ~BionicScopedTrace()
      {
        if (bionic_enable && tags)
        {
          char c = 'E';
          write(atrace_marker_fd, &c, 1);
          // TODO: what about closing the file?
        }
      }

    private:
      int atrace_marker_fd = -1;
      uint64_t tags;
      /* The value of bionic_enable corresponds to the value of ATRACE_TAG_BIONIC
       * defined in /system/core/include/cutils/trace.h.
       */
      uint64_t bionic_enable = ATRACE_TAG_BIONIC;
};
