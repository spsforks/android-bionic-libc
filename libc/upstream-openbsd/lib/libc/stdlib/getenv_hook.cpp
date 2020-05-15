// FIXME

// TODO: more intense checks to catch bugs
#include <map>
#include <mutex>

#include <log/log.h>

void hook_on_getenv(const char* key) {
  ALOGE("GETENV: %s", key);
}
void hook_on_setenv(const char* key, const char* value) {
  ALOGE("SETENV: %s %s", key, value);
}
void hook_on_unsetenv(const char* key) {
  ALOGE("UNSETENV: %s", key);
}
void hook_on_putenv(const char* keyvalue) {
  ALOGE("PUTENV: %s", keyvalue);
}

