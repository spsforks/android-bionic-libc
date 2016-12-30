/*
 * Copyright (C) 2016 The Android Open Source Project
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

/*
 * This file is only used to provide backwards compatibility to property areas
 * created by old versions of init, which occurs when an ota runs.  The updater
 * binary is compiled statically against the newest bionic, but the recovery
 * ramdisk may be using an old version of init.  This can all be removed once
 * OTAs from pre-O versions are no longer supported.
 */

#include <string.h>
#include <stdatomic.h>

#include "private/bionic_futex.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>


struct prop_info_compat {
    atomic_uint_least32_t serial;
    char value[PROP_VALUE_MAX];
    char name[0];
};

struct prop_msg_compat {
    unsigned cmd;
    char name[PROP_NAME_MAX];
    char value[PROP_VALUE_MAX];
};

typedef struct prop_info_compat prop_info_compat;
typedef struct prop_msg_compat prop_msg_compat;

static inline uint_least32_t load_const_atomic(const atomic_uint_least32_t* s,
                                               memory_order mo) {
    atomic_uint_least32_t* non_const_s = const_cast<atomic_uint_least32_t*>(s);
    return atomic_load_explicit(non_const_s, mo);
}

// Wait for non-locked serial, and retrieve it with acquire semantics.
static unsigned int read_serial(const prop_info_compat *pi)
{
    uint32_t serial = load_const_atomic(&pi->serial, memory_order_acquire);
    while (SERIAL_DIRTY(serial)) {
        __futex_wait(const_cast<volatile void *>(
                        reinterpret_cast<const void *>(&pi->serial)),
                     serial, NULL);
        serial = load_const_atomic(&pi->serial, memory_order_acquire);
    }
    return serial;
}

__LIBC_HIDDEN__ int __system_property_read_compat(const prop_info *_pi, char *name, char *value)
{
      const prop_info_compat* pi = reinterpret_cast<const prop_info_compat*>(_pi);
      while(true) {
          uint32_t serial = read_serial(pi); // acquire semantics
          size_t len = SERIAL_VALUE_LEN(serial);
          memcpy(value, pi->value, len + 1);
          atomic_thread_fence(memory_order_acquire);
          if (serial == load_const_atomic(&(pi->serial), memory_order_relaxed)) {
              if (name != 0) {
                  strcpy(name, pi->name);
                  return len;
              }
          }
      }
}

__LIBC_HIDDEN__ int __system_property_set_compat(const char *key, const char *value)
{
    if (key == 0) return -1;
    if (value == 0) value = "";
    if (strlen(key) >= PROP_NAME_MAX) return -1;
    if (strlen(value) >= PROP_VALUE_MAX) return -1;

    prop_msg_compat msg;
    memset(&msg, 0, sizeof msg);
    msg.cmd = PROP_MSG_SETPROP;
    strlcpy(msg.name, key, sizeof msg.name);
    strlcpy(msg.value, value, sizeof msg.value);

    const int err = __system_property_send_prop_msg(&msg, sizeof(prop_msg_compat));
    if (err < 0) {
        return err;
    }

    return 0;
}
