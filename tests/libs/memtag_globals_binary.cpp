#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>

static char exe_static[10];
static char* exe_static_end = &exe_static[17];

char exe_global[10];
static char* exe_global_end = &exe_global[17];

extern char dso_global[];
extern char* dso_global_end;

static const char exe_static_localstr[] = "this is a local string";

static _Atomic(size_t)* store;
static _Atomic(size_t) exe_static_atomic;
_Atomic(size_t) exe_global_atomic;

std::string exe_global_dyninit;
static std::string exe_static_dyninit;

union Union {
  int i;
  char c;
};

Union exe_global_union;
static Union exe_static_union;

// TODO(new): check new memtag note doesn't cause crash on old versions of android
void dso_print();
void dso_print_others();
void exe_print() {
  fprintf(stderr, "EXE dso_global:            %p\n", dso_global);
  fprintf(stderr, "EXE dso_global_end:        %p\n\n", dso_global_end);

  fprintf(stderr, "EXE exe_static:            %p\n", exe_static);
  fprintf(stderr, "EXE exe_static_end:        %p\n\n", exe_static_end);

  fprintf(stderr, "EXE exe_global:            %p\n", exe_global);
  fprintf(stderr, "EXE exe_global_end:        %p\n\n", exe_global_end);

  fprintf(stderr, "EXE &dso_global_end:       %p\n", &dso_global_end);
  fprintf(stderr, "EXE exe_static_localstr:   %p\n\n", exe_static_localstr);

  fprintf(stderr, "EXE &exe_global_atomic:    %p\n", &exe_global_atomic);
  fprintf(stderr, "EXE &exe_static_atomic:    %p\n\n", &exe_static_atomic);

  fprintf(stderr, "EXE &exe_global_dyninit:                    %p\n", &exe_global_dyninit);
  fprintf(stderr, "EXE exe_global_dyninit.data():              %p\n", exe_global_dyninit.data());
  exe_global_dyninit = "123456789012345678901234567890";
  fprintf(stderr, "EXE exe_global_dyninit.data() post-realloc: %p\n", exe_global_dyninit.data());
  fprintf(stderr, "EXE exe_static_dyninit.data():              %p\n", exe_static_dyninit.data());
  exe_global_dyninit = "123456789012345678901234567890";
  fprintf(stderr, "EXE exe_static_dyninit.data() post-realloc: %p\n\n", exe_static_dyninit.data());

  fprintf(stderr, "EXE &exe_global_union.i:       %p\n", &exe_global_union.i);
  fprintf(stderr, "EXE &exe_global_union.c:       %p\n", &exe_global_union.c);
  void* p = new (&exe_global_union.i) int;
  fprintf(stderr, "EXE &exe_global_union.i plnew: %p\n", p);
  exe_global_union.i = 123;
  fprintf(stderr, "EXE &exe_static_union.i:       %p\n", &exe_static_union.i);
  fprintf(stderr, "EXE &exe_static_union.c:       %p\n", &exe_static_union.c);

  static char exe_big_global[0x16000];
  fprintf(stderr, "EXE &exe_big_global:           %p\n\n", exe_big_global);
}

void check_matching_tags(void* a, void* b);
void dso_sanity_check();
void check_eq(const void* a, const void* b);

void exe_sanity_check() {
  *exe_static = 0;
  check_matching_tags(exe_static, exe_static_end);
  *exe_global = 0;
  check_matching_tags(exe_global, exe_global_end);
  dso_global[0] = 0;
  check_matching_tags(dso_global, dso_global_end);

  check_eq(exe_static + 17, exe_static_end);
  check_eq(exe_global + 17, exe_global_end);
  check_eq(dso_global + 17, dso_global_end);

  store = &exe_static_atomic;
  exe_static_atomic = 0;

  exe_global_union.i = 123;
  exe_global_union.c = '3';
  exe_static_union.i = 123;
  exe_static_union.c = '3';
}

void crash() {
  *dso_global_end = 0;
}

int main(int argc, char** argv) {
  if (argc > 1 && strcmp(argv[1], "heap") == 0) {
    volatile char* p = (volatile char*)malloc(1);
    fprintf(stderr, "doing heap testing: %p\n", p);
    free((void*)p);
    *p = 0;
    fprintf(stderr, "Failed heap sanity check.\n");
    exit(0);
  }
  dso_print();
  exe_print();
  dso_print_others();

  dso_sanity_check();
  exe_sanity_check();

  fprintf(stderr, "Passed sanity check, now crashing.\n");
  fprintf(stderr, "pid %i, waiting\n", getpid());

  crash();

  return 0;
}
