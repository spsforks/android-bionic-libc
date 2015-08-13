/*
 * Copyright 2015, The Android Open Source Project
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

#include <private/bionic_asm.h>

ENTRY(fma)
   vldr    d16, [sp]
   vmov    d18, r0, r1
   vmov    d19, r2, r3
   vmov.f64        d17, d16
   vmla.f64        d17, d18, d19
   vmov    r0, r1, d17
   bx      lr
END(fma)

ENTRY(fmaf)
   vmov    s14, r0
   vmov    s13, r2
   vmov    s15, r1
   vmla.f32        s13, s14, s15
   vmov    r0, s13
   bx      lr
END(fmaf)

ALIAS_SYMBOL(fmal, fma);
