#include "lib_static_init.h"
#include <stdio.h>

Foos::Foos()
{
    /* increment the static variable */
    value = ++Foos::counter;
    fprintf(stderr, "Foo::Foo for this=%p called (counter = %d)\n", this, counter);
}

int Foos::getValue()
{
    return value;
}

Foos::~Foos()
{
}


int Foos::counter;

Foos  theFoo;
