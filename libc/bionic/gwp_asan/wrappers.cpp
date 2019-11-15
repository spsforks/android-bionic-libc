#include <async_safe/log.h>
#include <stdarg.h>
#include <unistd.h>

namespace gwp_asan_wrappers {
void PrintfWrapper(const char *Format, ...) {
  va_list List;
  va_start(List, Format);
  async_safe_format_log_va_list(ANDROID_LOG_FATAL, "GWP-ASan", Format, List);
  va_end(List);
}
} // namespace gwp_asan_wrappers
