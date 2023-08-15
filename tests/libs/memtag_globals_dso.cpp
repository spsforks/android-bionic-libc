#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

static char dso_static[10];
static char* dso_static_end = &dso_static[17];
static const char* dso_static_end_const = &dso_static[17];

char dso_global[10];
char* dso_global_end = &dso_global[17];
static const char* dso_global_end_const = &dso_global[17];

class Foo {
 public:
  virtual ~Foo() {}
  static const std::string dso_static_string;
  virtual const char* f() const { return dso_static_string.c_str(); }
};
const std::string Foo::dso_static_string("12345");

extern __attribute__((visibility("hidden"))) int dso2_variable;

void assertion_failure() {
  fprintf(stderr, "MEMTAG_GLOBALS_FAILURE\n");
  exit(1);
}

void dso_print() {
  fprintf(stderr, "DSO dso_static:            %p\n", dso_static);
  fprintf(stderr, "DSO dso_static_end:        %p\n", dso_static_end);
  fprintf(stderr, "DSO dso_static_end_const:  %p\n\n", dso_static_end_const);

  fprintf(stderr, "DSO dso_global:            %p\n", dso_global);
  fprintf(stderr, "DSO dso_global_end:        %p\n", dso_global_end);
  fprintf(stderr, "DSO dso_global_end_const:  %p\n", dso_global_end_const);

  fprintf(stderr, "DSO dso2_variable:         %p\n", &dso2_variable);
}

void dso_print_others() {
  fprintf(stderr, "DSO &dso_global_end_const: %p\n", &dso_global_end_const);
  fprintf(stderr, "DSO &dso_static_end_const: %p\n", &dso_static_end_const);
  fprintf(stderr, "DSO &dso_global_end:       %p\n", &dso_global_end);
  fprintf(stderr, "DSO &dso_static_end:       %p\n", &dso_static_end);

  Foo foo;
  fprintf(stderr, "DSO &static_string:        %p\n", foo.f());
}

void check_matching_tags(void* a, void* b) {
  uintptr_t a_uptr = (uintptr_t)a;
  uintptr_t b_uptr = (uintptr_t)b;
  if (a_uptr >> 56 != b_uptr >> 56) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  tag(0x%zx) != tag(0x%zx)\n", a_uptr, b_uptr);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
}

void check_eq(const void* a, const void* b) {
  if (a != b) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  %p != %p\n", a, b);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
}

void dso_check_assertions() {
  dso_static[0] = 0;
  check_matching_tags(dso_static, dso_static_end);
  dso_global[0] = 0;
  check_matching_tags(dso_global, dso_global_end);

  check_eq(dso_static + 17, dso_static_end);
  check_eq(dso_static + 17, dso_static_end_const);
  check_eq(dso_global + 17, dso_global_end);
  check_eq(dso_global + 17, dso_global_end_const);
}

void write_dso_static() {
  dso_static[0] = 0;
}

void write_dso_static_end() {
  dso_static_end[0] = 0;
}

void write_dso_global() {
  dso_global[0] = 0;
}

void write_dso_global_end() {
  dso_global_end[0] = 0;
}

void read_dso_static_end_const() {
  volatile char f = dso_static_end_const[0];
}

void read_dso_global_end_const() {
  volatile char f = dso_global_end_const[0];
}
