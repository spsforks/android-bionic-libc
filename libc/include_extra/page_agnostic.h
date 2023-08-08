
#ifdef PAGE_SIZE_AGNOSTIC
#error WIP, This case should happen in page_agnostic.h PA
#define __BIONIC_NO_PAGE_SIZE_MACRO
#else
#error WIP, This case should not happen in page_agnostic.h
#endif

#include_next <sys/user.h>
