/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef BIONIC_BENCHMARK_HISTOGRAM_UTILS_H_
#define BIONIC_BENCHMARK_HISTOGRAM_UTILS_H_

#include <stdarg.h>

#define CHECK(condition)
#define CHECK_LT(val1, val2)
#define CHECK_GE(val1, val2)

#define DCHECK_EQ(val1, val2)
#define DCHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2)

namespace histogram {
enum TimeUnit {
    kTimeUnitNanosecond,
    kTimeUnitMicrosecond,
    kTimeUnitMillisecond,
    kTimeUnitSecond,
};

static void StringAppendV(std::string* dst, const char* format, va_list ap) {
  // First try with a small fixed size buffer
  char space[1024];

  // It's possible for methods that use a va_list to invalidate
  // the data in it upon use.  The fix is to make a copy
  // of the structure before using it and use that copy instead.
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int result = vsnprintf(space, sizeof(space), format, backup_ap);
  va_end(backup_ap);

  if (result < static_cast<int>(sizeof(space))) {
    if (result >= 0) {
      // Normal case -- everything fit.
      dst->append(space, result);
      return;
    }

    if (result < 0) {
      // Just an error.
      return;
    }
  }

  // Increase the buffer size to the size requested by vsnprintf,
  // plus one for the closing \0.
  int length = result+1;
  char* buf = new char[length];

  // Restore the va_list before we use it again
  va_copy(backup_ap, ap);
  result = vsnprintf(buf, length, format, backup_ap);
  va_end(backup_ap);

  if (result >= 0 && result < length) {
    // It fit
    dst->append(buf, result);
  }
  delete[] buf;
}

static std::string StringPrintf(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  std::string result;
  StringAppendV(&result, fmt, ap);
  va_end(ap);
  return result;
}

static TimeUnit GetAppropriateTimeUnit(uint64_t nano_duration) {
  const uint64_t one_sec = 1000 * 1000 * 1000;
  const uint64_t one_ms  = 1000 * 1000;
  const uint64_t one_us  = 1000;
  if (nano_duration >= one_sec) {
    return kTimeUnitSecond;
  } else if (nano_duration >= one_ms) {
    return kTimeUnitMillisecond;
  } else if (nano_duration >= one_us) {
    return kTimeUnitMicrosecond;
  } else {
    return kTimeUnitNanosecond;
  }
}

static uint64_t GetNsToTimeUnitDivisor(TimeUnit time_unit) {
  const uint64_t one_sec = 1000 * 1000 * 1000;
  const uint64_t one_ms  = 1000 * 1000;
  const uint64_t one_us  = 1000;

  switch (time_unit) {
    case kTimeUnitSecond:
      return one_sec;
    case kTimeUnitMillisecond:
      return one_ms;
    case kTimeUnitMicrosecond:
      return one_us;
    case kTimeUnitNanosecond:
      return 1;
  }
  return 0;
}

static std::string FormatDuration(uint64_t nano_duration, TimeUnit time_unit) {
  const char* unit = NULL;
  uint64_t divisor = GetNsToTimeUnitDivisor(time_unit);
  uint32_t zero_fill = 1;
  switch (time_unit) {
    case kTimeUnitSecond:
      unit = "s";
      zero_fill = 9;
      break;
    case kTimeUnitMillisecond:
      unit = "ms";
      zero_fill = 6;
      break;
    case kTimeUnitMicrosecond:
      unit = "us";
      zero_fill = 3;
      break;
    case kTimeUnitNanosecond:
      unit = "ns";
      zero_fill = 0;
      break;
  }

  uint64_t whole_part = nano_duration / divisor;
  uint64_t fractional_part = nano_duration % divisor;
  if (fractional_part == 0) {
    return StringPrintf("%llu%s", whole_part, unit);
  } else {
    while ((fractional_part % 1000) == 0) {
      zero_fill -= 3;
      fractional_part /= 1000;
    }
    if (zero_fill == 3) {
      return StringPrintf("%llu.%03llu%s", whole_part, fractional_part, unit);
    } else if (zero_fill == 6) {
      return StringPrintf("%llu.%06llu%s", whole_part, fractional_part, unit);
    } else {
      return StringPrintf("%llu.%09llu%s", whole_part, fractional_part, unit);
    }
  }
}

}  // namespace histogram
#endif  // BIONIC_BENCHMARK_HISTOGRAM_UTILS_H_
