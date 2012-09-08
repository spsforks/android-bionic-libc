/*
 * Copyright (C) 2012 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <malloc.h>
#include <pthread.h>

// libstdc++ currently contains __cxa_guard_acquire and __cxa_guard_release,
// so we make do with macros instead of the commented-out class below.
// TODO: move __cxa_guard_acquire and __cxa_guard_release into libc.

#define GLOBAL_INIT_THREAD_LOCAL_BUFFER(name, byte_count) \
  static pthread_once_t name ## _once; \
  static pthread_key_t name ## _key; \
  static void name ## _key_destroy(void* buffer) { \
    free(buffer); \
  } \
  static void name ## _key_init() { \
    pthread_key_create(&name ## _key, name ## _key_destroy); \
    void* buffer = malloc(byte_count); \
    pthread_setspecific(name ## _key, buffer); \
  } \
  static const size_t name ## _buffer_size = byte_count

// Leaves "name_buffer" and "name_byte_count" defined and initialized.
#define LOCAL_INIT_THREAD_LOCAL_BUFFER(type, name) \
  pthread_once(&name ## _once, name ## _key_init); \
  type name ## _buffer = reinterpret_cast<type>(pthread_getspecific(name ## _key))

/*
// Use like this:
//   const char* f() {
//     static ThreadLocalBuffer<char, PATH_MAX> buffer;
//     char* p = buffer.get();
//     ...
//     return p;
//   }
template <typename T, size_t BYTE_COUNT>
class ThreadLocalBuffer {
 public:
  ThreadLocalBuffer() {
    pthread_key_create(&key_, Destroy);
    void* buffer = malloc(BYTE_COUNT);
    pthread_setspecific(key_, buffer);
  }

  T* get() {
    return reinterpret_cast<T*>(pthread_getspecific(key_));
  }

  size_t size() const {
    return BYTE_COUNT;
  }

 private:
  pthread_key_t key_;

  static void Destroy(void* buffer) {
    free(buffer);
    //pthread_setspecific(key_, NULL);
  }

  ThreadLocalBuffer(const ThreadLocalBuffer&);
  void operator=(const ThreadLocalBuffer&);
};
*/
