/*
 * Copyright 2010-2014, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fd.h"

__EXPORT double sqrt(double x)
{
    double r;
    __asm__ __volatile__("fsqrt %d0, %d1": "=x"(r): "x"(x));
    return r;
}

__EXPORT float sqrtf(float x)
{
    float r;
    __asm__ __volatile__("fsqrt %s0, %s1": "=x"(r): "x"(x) );
    return r;
}
