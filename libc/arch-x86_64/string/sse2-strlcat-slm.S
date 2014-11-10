#define USE_AS_STRLCAT

#ifndef STRLCPY
# define STRLCPY	strlcat
#endif

#include "sse2-strlcpy-slm.S"
