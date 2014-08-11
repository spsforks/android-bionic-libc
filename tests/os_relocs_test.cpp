/* this little test is written to check that the relocations generated
 * in a shared library are correct. it depends on the content of lib_relocs.c
 * being compiled as a shared object.
 */
#include <stdio.h>
#include <gtest/gtest.h>

extern int  func1(void);
extern int  func2(void);

TEST(os, relocs)
{
    int   f1, f2, expect1 = 1, expect2 = 2;

    f1 = func1();
    f2 = func2();

    if (f1 != expect1 || f2 != expect2){
       ADD_FAILURE();
    }

}
