#include <link.h>
#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>

#include "libs_utils.h"

extern "C" void _start();
const char* helper_func();

int main(int argc, char* argv[]) {
  printf("argc=%d argv[0]=%s\n", argc, argv[0]);
  printf("AT_EXECFN=%s\n", reinterpret_cast<const char*>(getauxval(AT_EXECFN)));
  printf("%s\n", helper_func());

  CHECK(getauxval(AT_BASE) != 0);
  CHECK(getauxval(AT_EXECFD) == 0);
  CHECK(getauxval(AT_ENTRY) == reinterpret_cast<unsigned long>(&_start));

  dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void*) {
    if (info->dlpi_phdr == reinterpret_cast<const ElfW(Phdr)*>(getauxval(AT_PHDR)) &&
        info->dlpi_phnum == getauxval(AT_PHNUM)) {
      printf("dl_iterate_phdr match: %s\n", info->dlpi_name);
    }
    return 0;
  }, nullptr);

  return 0;
}
