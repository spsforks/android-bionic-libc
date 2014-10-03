#include <stdio.h>

int foo(void) {
  char buf[2];
  // GCC: no return statement in function returning non-void [-Werror=return-type]
  // CLANG: control reaches end of non-void function [-Werror,-Wreturn-type]
  sprintf(buf, "%s", "foobar");
}
