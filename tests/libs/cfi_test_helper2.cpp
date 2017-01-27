#include <assert.h>
#include <dlfcn.h>

int main(void) {
  void* handle;
  // libcfi-test.so does some basic testing in a global constructor. Check that it is linked.
  handle = dlopen("libcfi-test.so", RTLD_NOW | RTLD_NOLOAD);
  assert(handle != nullptr);
  dlclose(handle);
  return 0;
}
