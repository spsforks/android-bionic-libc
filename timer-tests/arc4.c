#include <stdio.h>
#include <stdlib.h>

int main() {
  for (size_t i = 0; i < 10; ++i) {
     fprintf(stderr, "%08x\n", arc4random());
  }
  return 0;
}
