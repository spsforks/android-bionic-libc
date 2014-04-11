
// TODO: move out of here and add Android header!

#include <pthread.h>

// TODO: hidden visibility!
pthread_mutex_t __dtoa_locks[] = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };



// TODO: remove bionic/strtod.c too!
