/*
 * Copyright (C) 2016 The Android Open Source Project
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

#pragma once

#define __MUTATED_API_LEVEL(api_level) (api_level)

// The `annotate` attribute always pulls the annotated (inline) function into the object files, thus
// we should only annotate headers when we are running versioner.
#if defined(__BIONIC_VERSIONER)

#define __INTRODUCED_IN(api_level) __attribute__((annotate("introduced_in=" #__MUTATED_API_LEVEL(api_level))))
#define __DEPRECATED_IN(api_level) __attribute__((annotate("deprecated_in=" #__MUTATED_API_LEVEL(api_level))))
#define __REMOVED_IN(api_level) __attribute__((annotate("obsoleted_in=" #__MUTATED_API_LEVEL(api_level))))
#define __INTRODUCED_IN_32(api_level) __attribute__((annotate("introduced_in_32=" #__MUTATED_API_LEVEL(api_level))))
#define __INTRODUCED_IN_64(api_level) __attribute__((annotate("introduced_in_64=" #__MUTATED_API_LEVEL(api_level))))
#define __INTRODUCED_IN_ARM(api_level) __attribute__((annotate("introduced_in_arm=" #__MUTATED_API_LEVEL(api_level))))
#define __INTRODUCED_IN_X86(api_level) __attribute__((annotate("introduced_in_x86=" #__MUTATED_API_LEVEL(api_level))))

#define __VERSIONER_NO_GUARD __attribute__((annotate("versioner_no_guard")))
#define __VERSIONER_FORTIFY_INLINE __attribute__((annotate("versioner_fortify_inline")))

#else

#define __INTRODUCED_IN(api_level)
#define __DEPRECATED_IN(api_level)
#define __REMOVED_IN(api_level)
#define __INTRODUCED_IN_32(api_level)
#define __INTRODUCED_IN_64(api_level)
#define __INTRODUCED_IN_ARM(api_level)
#define __INTRODUCED_IN_X86(api_level)

#define __VERSIONER_NO_GUARD
#define __VERSIONER_FORTIFY_INLINE

#endif  // defined(__BIONIC_VERSIONER)

#undef __MUTATED_API_LEVEL
