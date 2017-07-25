/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _BIONIC_BENCHMARKS_TIME_BENCHMARK_H
#define _BIONIC_BENCHMARKS_TIME_BENCHMARK_H

#include <map>
#include <string>

#include "util.h"

void declare_time_benchmarks(std::map <std::string, benchmark_func_t>& str_to_func);

#endif // _BIONIC_BENCHMARKS_TIME_BENCHMARK_H
