// FIXME

#include "getenv_checks.h"

#include <dlfcn.h>

// I didn't check if any calls here recurse, but in case they do, just ignore
// them

static void* last_stack_address = nullptr;
static bool avoid_recur() {
    void* sp;
    (void) sp;
    if (last_stack_address) {
        if (&sp < last_stack_address) {
            return true;
        }
    }
    last_stack_address = &sp;
    return false;
}
static void reset_recur() {
    last_stack_address = nullptr;
}

static decltype(on_getenv)* g_hook_on_getenv = nullptr;
static decltype(on_setenv)* g_hook_on_setenv = nullptr;
static decltype(on_unsetenv)* g_hook_on_unsetenv = nullptr;
static decltype(on_putenv)* g_hook_on_putenv = nullptr;

__attribute__((weak)) void* dlsym(void* handle, const char* symbol);
__attribute__((weak)) void* dlopen(const char* filename, int flag);

static void install() {
    static bool once = []() {
        if (dlsym == nullptr) return false;
        if (dlopen == nullptr) return false;
        void* handle = dlopen("libc_env_hooks.so", RTLD_NOW);
        if (handle == nullptr) return false;
        g_hook_on_getenv = (decltype(on_getenv)*) dlsym(handle, "hook_on_getenv");
        g_hook_on_setenv = (decltype(on_setenv)*) dlsym(handle, "hook_on_setenv");
        g_hook_on_unsetenv = (decltype(on_unsetenv)*) dlsym(handle, "hook_on_unsetenv");
        g_hook_on_putenv = (decltype(on_putenv)*) dlsym(handle, "hook_on_putenv");
        return true;
    }();
    (void) once;
}

void on_getenv(const char* key) {
    if (avoid_recur()) return;
    install();
    if (g_hook_on_getenv != nullptr) g_hook_on_getenv(key);
    reset_recur();
}
void on_setenv(const char* key, const char* value) {
    if (avoid_recur()) return;
    install();
    if (g_hook_on_setenv != nullptr) g_hook_on_setenv(key, value);
    reset_recur();
}
void on_unsetenv(const char* key) {
    if (avoid_recur()) return;
    install();
    if (g_hook_on_unsetenv != nullptr) g_hook_on_unsetenv(key);
    reset_recur();
}
void on_putenv(const char* keyvalue) {
    if (avoid_recur()) return;
    install();
    if (g_hook_on_putenv != nullptr) g_hook_on_putenv(keyvalue);
    reset_recur();
}

