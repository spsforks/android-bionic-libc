#define IEEE_8087
#define Arith_Kind_ASL 1

#if __LP64__
#define Long int
#define Intcast (int)(long)
#define Double_Align
#define X64_bit_pointers
#endif

#if 0 /* TODO: define these here instead of in the makefile? */
#define INFNAN_CHECK
#define MULTIPLE_THREADS
#define NO_FENV_H
#endif
