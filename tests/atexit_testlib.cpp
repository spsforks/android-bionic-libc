#include <stdio.h>
#include <stdlib.h>

#include "../libc/stdlib/atexit.h"

// use external control number from main test
static int* _control_atexit = NULL;
static int* _control_staticdtor = NULL;

class AtExitStaticClass;

static const AtExitStaticClass* _remember_this = NULL;

static class AtExitStaticClass {
public:
  AtExitStaticClass() { _remember_this = this; }
  ~AtExitStaticClass() {
    if (_control_staticdtor) {
      *_control_staticdtor = (_remember_this == this) ? 1 : -1 ;
    }
  }
} staticObj;

// this one should be second
static void atexit_handler_from_atexit() {
  *_control_atexit *= 3;
}

extern struct atexit *__atexit;

// this goes first
static void atexit_handler_with_atexit() {
  static const char* prefix = "atexit_handler_with_atexit";
  *_control_atexit = 1;
  atexit(atexit_handler_from_atexit);
}

// this should be called last
static void atexit_handler_regular() {
  *_control_atexit += 5; // control is 15
}

extern "C" void register_atexit(int* control_atexit, int* control_staticdtor) {
  _control_atexit = control_atexit;
  _control_staticdtor = control_staticdtor;
  atexit(atexit_handler_regular);
  atexit(atexit_handler_with_atexit);
  atexit(NULL);
}

