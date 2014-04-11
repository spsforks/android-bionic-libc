#if __arm__

#define f_QNAN  0xffffffff

#define d_QNAN0 0xffffffff
#define d_QNAN1 0xffffffff

#define ldus_QNAN0 0xffff
#define ldus_QNAN1 0xffff
#define ldus_QNAN2 0xffff
#define ldus_QNAN3 0xffff
#define ldus_QNAN4 0x0000

#elif __mips__

#define f_QNAN  0x7fbfffff

#define d_QNAN0 0x7ff7ffff
#define d_QNAN1 0xffffffff

#define ldus_QNAN0 0x7fff
#define ldus_QNAN1 0x8000
#define ldus_QNAN2 0x0000
#define ldus_QNAN3 0x0000
#define ldus_QNAN4 0x0000

#else

#define f_QNAN  0xffc00000

#define d_QNAN0 0x00000000
#define d_QNAN1 0xfff80000

#define ldus_QNAN0 0x0000
#define ldus_QNAN1 0x0000
#define ldus_QNAN2 0x0000
#define ldus_QNAN3 0xc000
#define ldus_QNAN4 0xffff

#endif
