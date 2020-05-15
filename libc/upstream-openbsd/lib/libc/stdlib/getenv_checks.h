// FIXME

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void on_getenv(const char* key);
void on_setenv(const char* key, const char* value);
void on_unsetenv(const char* key);
void on_putenv(const char* keyvalue);

#ifdef __cplusplus
}
#endif

