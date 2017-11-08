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

#ifndef SYSTEM_PROPERTIES_CONTEXTS_SPLIT_H
#define SYSTEM_PROPERTIES_CONTEXTS_SPLIT_H

#include <stdlib.h>
#include <string.h>

#include "context_node.h"
#include "contexts.h"

struct PrefixNode;
class ContextListNode : public ContextNode {
 public:
  ContextListNode(ContextListNode* next, const char* context)
      : ContextNode(strdup(context)), next(next) {
  }

  ~ContextListNode() {
    free(const_cast<char*>(context()));
  }

  ContextListNode* next;
};

class ContextsSplit : public Contexts {
 public:
  virtual ~ContextsSplit() override {
  }

  virtual bool Initialize(bool writable) override;
  virtual prop_area* GetPropAreaForName(const char* name) override;
  virtual void ForEach(void (*propfn)(const prop_info* pi, void* cookie), void* cookie) override;
  virtual void ResetAccess() override;
  virtual void FreeAndUnmap() override;

 private:
  bool InitializePropertiesFromFile(const char* filename);
  bool InitializeProperties();

  PrefixNode* prefixes_ = nullptr;
  ContextListNode* contexts_ = nullptr;
};

#endif
