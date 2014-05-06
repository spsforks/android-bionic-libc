#include <stdlib.h>
#include <stdio.h>

class TestMainStaticDtorClass;

static const TestMainStaticDtorClass* valid_this = nullptr;

static class TestMainStaticDtorClass {
public:
  TestMainStaticDtorClass() {
    valid_this = this;
  }

  ~TestMainStaticDtorClass() {
    if (this != valid_this) {
      printf("\nerror: invalid this in static d-tor: %p, expected: %p\n", this, valid_this);
    }
  }

} testMainVar;

void func5() {
  printf(" and gimble in the wabe;");
}

void func4() {
  printf(" toves did gyre");
}

void func3() {
  printf(" and the slithy");
  atexit(func4);
}

void func2() {
  printf(" brillig,");
}

void func1() {
  printf("Twas");
}

int main() {
  // This should result in "Twas brillig, and the slithy toves did gyre and gimble in the wabe;"
  atexit(func5);
  atexit(func3);
  atexit(func2);
  atexit(func1);
  return 0;
}
