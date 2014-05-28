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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cutils/compiler.h>
#include <utils/threads.h>
#include <cutils/trace.h>

/* Tracing class for bionic.  To enable bionic tracing:
 *   #define ATRACE_TAG ATRACE_TAG_BIONIC
 *
 * To begin a trace at a specified point:
 *   android::BionicScopedTrace(ATRACE_TAG, "Trace message");
 * The trace will end when the contructor goes out of scope, or
 * when the destructor is called.
 */

#define ATRACE_MESSAGE_LENGTH 1024
extern uint64_t atrace_enabled_tags;

namespace android {
  class BionicScopedTrace {
    public:
      inline BionicScopedTrace(uint64_t tag, const char* name): mTag(tag)
      {
        if(atrace_marker_fd == -1)
          atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);

        if(atrace_marker_fd == -1)
        {
          fprintf(stderr, "Could not open kernel trace file\n");
          fflush(stderr);
          //atrace_enabled_tags = 0;
        }
        else
        {
          // TODO: check for tag enable
          //if (CC_UNLIKELY(atrace_is_tag_enabled(tag)))
          //{
            char buf[ATRACE_MESSAGE_LENGTH];
            size_t len;

            len = snprintf(buf, ATRACE_MESSAGE_LENGTH, "B|%d|%s", getpid(), name);
            write(atrace_marker_fd, buf, len);
          //}
        }
      }

      inline ~BionicScopedTrace()
      {
        // TODO: same as above
        //if (CC_UNLIKELY(atrace_is_tag_enabled(tag)))
        //{
          char c = 'E';
          write(atrace_marker_fd, &c, 1);
        //}
      }

    private:
      uint64_t mTag;
      int atrace_marker_fd = -1;
  };
};
