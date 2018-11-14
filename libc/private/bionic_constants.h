/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BIONIC_CONSTANTS_H_
#define _BIONIC_CONSTANTS_H_

#define NS_PER_S 1000000000

// Size of the shadow call stack.
#define SCS_SIZE (8 * 1024)

// The shadow call stack is allocated at an aligned address within a guard region
// of this size.
// TODO(b/118642754): Use a larger guard region.
#define SCS_GUARD_REGION_SIZE (12 * 1024)

#endif // _BIONIC_CONSTANTS_H_
