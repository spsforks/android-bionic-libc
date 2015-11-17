
#include <pthread.h>

#include <private/bionic_malloc_debug.h>

#include "DebugData.h"
#include "debug_disable.h"

extern DebugData* g_debug;
pthread_key_t g_disable_key;

bool DebugCallsDisabled() {
  if (g_debug == nullptr || pthread_getspecific(g_disable_key) != nullptr) {
    return true;
  }
  return false;
}

bool DebugDisableInitialize() {
  int error = pthread_key_create(&g_disable_key, nullptr);
  if (error != 0) {
    error_log("pthread_key_create failed: %s", strerror(error));
    return false;
  }
  pthread_setspecific(g_disable_key, nullptr);

  return true;
}

void DebugDisableFinalize() {
  pthread_key_delete(g_disable_key);
}

void DebugDisableSet(bool disable) {
  if (disable) {
    pthread_setspecific(g_disable_key, reinterpret_cast<void*>(1));
  } else {
    pthread_setspecific(g_disable_key, nullptr);
  }
}
