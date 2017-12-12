/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef SYSTEM_PROPERTIES_SYSTEM_PROPERTIES_H
#define SYSTEM_PROPERTIES_SYSTEM_PROPERTIES_H

constexpr int PROP_FILENAME_MAX = 1024;

class SystemProperties {
 public:
  int Init(const char* filename);
  int AreaInit(const char* filename);
  uint32_t AreaSerial();
  const prop_info* Find(const char* name);
  int Read(const prop_info* pi, char* name, char* value);
  void ReadCallback(const prop_info* pi,
                    void (*callback)(void* cookie, const char* name, const char* value,
                                     uint32_t serial),
                    void* cookie);
  int Get(const char* name, char* value);
  int Update(prop_info* pi, const char* value, unsigned int len);
  int Add(const char* name, unsigned int namelen, const char* value, unsigned int valuelen);
  uint32_t Serial(const prop_info* pi);
  uint32_t WaitAny(uint32_t old_serial);
  bool Wait(const prop_info* pi, uint32_t old_serial, uint32_t* new_serial_ptr,
            const timespec* relative_timeout);
  const prop_info* FindNth(unsigned n);
  int Foreach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie);

 private:
  // We don't want to use new or malloc in properties (b/31659220), and since these classes are
  // small enough and we place them in a static union.  Note that system properties are initialized
  // before static initializers are called, so using a Constructor here is an error.  Even a
  // Constructor that zero initializes a class will clobber the previous property initialization.
  static union ContextsUnion {
    ContextsUnion() {
    }
    ~ContextsUnion() {
    }
    ContextsSerialized contexts_serialized;
    ContextsSplit contexts_split;
    ContextsPreSplit contexts_pre_split;
  } contexts_union_;
  Contexts* contexts_;
  char property_filename_[PROP_FILENAME_MAX];
};

#endif
