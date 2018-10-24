#include <string.h>

__attribute__ ((noinline))
int test2(const char* a, const char* b) {
    return strcmp(a, b);
}

int main() {
    return test2("abc", "abc");
}

