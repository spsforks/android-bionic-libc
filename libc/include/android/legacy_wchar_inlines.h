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

#ifndef ANDROID_LEGACY_WCHAR_INLINES_H
#define ANDROID_LEGACY_WCHAR_INLINES_H

#include <sys/cdefs.h>
#include <wchar.h>

__BEGIN_DECLS

#if __ANDROID_API__ < 21

static __inline int wcscoll_l(const wchar_t* ws1, const char* ws2, locale_t l __unused) {
  return wcscoll(ws1, ws2);
}

size_t wcsxfrm_l(wchar_t* dest, const char* src, size_t n, locale_t l __unused) {
  return wcsxfrm(dest, src, n);
}

#endif /* __ANDROID_API__ < 21 */

#if __ANDROID_API__ < __ANDROID_API_FUTURE__

static inline long long wcstoll_l(const wchar_t* nptr, wchar_t** endptr, int base,
                                  locale_t l __unused) {
  return wcstoll(nptr, endptr, base);
}

static inline unsigned long long wcstoull_l(const wchar_t* nptr, wchar_t** endptr, int base,
                                            locale_t l __unused) {
  return wcstoull(nptr, endptr, base);
}

static inline long double wcstold_l(const wchar_t* nptr, wchar_t** endptr, locale_t l __unused) {
  return wcstold(nptr, endptr);
}

#endif /* __ANDROID_API__ < __ANDROID_API_FUTURE__ */

__END_DECLS

#endif /* ANDROID_LEGACY_WCHAR_INLINES_H */
