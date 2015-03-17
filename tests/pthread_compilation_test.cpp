#include <pthread.h>

void pthread_mutex_t_alignment() {
  struct checked_type {
    char a;
    pthread_mutex_t mutex;
  };
  static_assert(sizeof(pthread_mutex_t) % 4 == 0,
    "pthread_mutex_t should fulfill alignment needed by implementation.");
  static_assert(sizeof(checked_type) % 4 == 0,
    "pthread_mutex_t should fulfill alignment needed by implementation.");
}

void pthread_cond_t_alignment() {
  struct checked_type {
    char a;
    pthread_cond_t cond;
  };
  static_assert(sizeof(pthread_cond_t) % 4 == 0,
    "pthread_cond_t should fulfill alignment needed by implementation.");
  static_assert(sizeof(checked_type) % 4 == 0,
    "pthread_cond_t should fulfill alignment needed by implementation.");
}

void pthread_rwlock_t_alignment() {
  struct checked_type {
    char a;
    pthread_rwlock_t rwlock;
  };
  static_assert(sizeof(pthread_rwlock_t) % 4 == 0,
    "pthread_rwlock_t should fulfill alignment needed by implementation.");
  static_assert(sizeof(checked_type) % 4 == 0,
    "pthread_rwlock_t should fulfill alignment needed by implementation.");
}
