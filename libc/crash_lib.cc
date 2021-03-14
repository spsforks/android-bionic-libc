#include <stdio.h>
#include <stdlib.h>

void __attribute__ ((constructor)) crash() {
    abort();
}

extern "C" void hello() {
    printf("Hello!");
}
