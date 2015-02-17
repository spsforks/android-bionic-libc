/*
 * Copyright 2010-2015, The Android Open Source Project
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

#include "math.h"


#define ROUNDD(dtype, fn, roundit) \
dtype fn(double x)                                             \
{                                                                       \
    double r;                                                           \
    __asm__ __volatile__( #roundit " %d0, %d1": "=x"(r): "x"(x));       \
    return (dtype) r;                                                   \
}

#define ROUNDF(dtype, fn, roundit) \
dtype fn(float x)                                              \
{                                                                       \
    float r;                                                            \
    __asm__ __volatile__( #roundit " %s0, %s1": "=x"(r): "x"(x));       \
    return (dtype) r;                                                   \
}

ROUNDD(double, rint,  frintX);
ROUNDF(float,  rintf, frintX);

ROUNDD(long, lrint,  frintX);
ROUNDF(long, lrintf, frintX);

ROUNDD(long long, llrint,  frintX);
ROUNDF(long long, llrintf, frintX);

ROUNDD(double, ceil, frintP);
ROUNDF(float,  ceilf, frintP);

ROUNDD(double, floor, frintM);
ROUNDF(float,  floorf, frintM);

ROUNDD(double, trunc,  frintZ);
ROUNDF(float,  truncf, frintZ);
