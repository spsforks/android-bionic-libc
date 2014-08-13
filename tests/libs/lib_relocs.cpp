/* this is part of the test_relocs.c test, which is used to check that
 * the relocations generated in a shared object are properly handled
 * by the Bionic dynamic linker
 */


struct foo { int first, second; };
struct foo Foo = {1, 2};

int* FooPtr[] = { &Foo.first, &Foo.second };

int relocs_func1() {
  return *FooPtr[0];
}

int relocs_func2() {
  return *FooPtr[1];
}
