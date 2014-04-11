#ifndef _THREAD_PRIVATE_H_
#define _THREAD_PRIVATE_H_

#include <pthread.h>

/* Note that these aren't compatible with the usual OpenBSD ones which lazy-initialize! */
#define _MUTEX_LOCK(l) pthread_mutex_lock((pthread_mutex_t*) l)
#define _MUTEX_UNLOCK(l) pthread_mutex_unlock((pthread_mutex_t*) l)

#endif /* _THREAD_PRIVATE_H_ */
