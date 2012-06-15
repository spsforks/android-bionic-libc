/* These are stub functions that are actually defined
 * in the dynamic linker (aprof.c), and hijacked at runtime.
 */
#include <stdint.h>
void aprof_init(){}
void aprof_fini(){}
void aprof_mcount(uintptr_t frompc, uintptr_t selfpc){}
void aprof_control(int c){};
