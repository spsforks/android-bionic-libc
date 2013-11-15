/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef BIONIC_BENCHMARK_H_
#define BIONIC_BENCHMARK_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

namespace testing {

class Benchmark;

void BenchmarkRegister(Benchmark* bm);
int PrettyPrintInt(char* str, int len, int64_t arg);

class Benchmark {
 public:
  Benchmark(const char* name, void (*fn)(int)) : name_(strdup(name)), fn_(fn) {
    BenchmarkRegister(this);
  }
  Benchmark(const char* name) : name_(strdup(name)), fn_(NULL) {}

  virtual ~Benchmark() {
    free(name_);
  }

  const char* Name();
  virtual const char* ArgName() { return NULL; }
  virtual void RunFn(int iterations) { fn_(iterations); }
  bool ShouldRun(int argc, char* argv[]);
  void Run();
  void RunIterations(int iterations);

 private:
  char* name_;

  void (*fn_)(int);
};

template <typename T>
class BenchmarkWithArgBase : public Benchmark {
 public:
  BenchmarkWithArgBase(const char* name, void (*fn)(int, T)) : Benchmark(name) {
    fn_arg_ = fn;
    arg_name_ = NULL;
  }

  BenchmarkWithArgBase(const char* name, void (*fn)(int, T), const char* arg_name, T arg) : Benchmark(name) {
    fn_arg_ = fn;
    arg_name_ = strdup(arg_name);
    arg_ = arg;
  }

  virtual ~BenchmarkWithArgBase() {
    free(arg_name_);
  }

  virtual const char* ArgName() { return arg_name_; }

  virtual void RunFn(int iterations) {
    if (arg_name_) {
      fn_arg_(iterations, arg_);
    } else {
      printf("can't run arg benchmark %s without arg\n", Name());
    }
  }

protected:
  void (*fn_arg_)(int, T);
  char* arg_name_;
  T arg_;
};

template <typename T>
class BenchmarkWithArg : public BenchmarkWithArgBase<T> {
 public:
  BenchmarkWithArg<T>(const char* name, void (*fn)(int, T)) :
    BenchmarkWithArgBase<T>(name, fn) { }

  BenchmarkWithArg<T>(const char* name, void (*fn)(int, T), const char* arg_name, T arg) :
    BenchmarkWithArgBase<T>(name, fn, arg_name, arg) { }

  BenchmarkWithArg<T>* Arg(const char* arg_name, T arg) {
    BenchmarkRegister(new BenchmarkWithArg<T>(BenchmarkWithArgBase<T>::name_,
                                              BenchmarkWithArgBase<T>::fn_arg_,
                                              arg_name, arg));
    return this;
  }
};

template <>
class BenchmarkWithArg<int> : public BenchmarkWithArgBase<int> {
 public:
  BenchmarkWithArg<int>(const char* name, void (*fn)(int, int)) :
    BenchmarkWithArgBase<int>(name, fn) { }

  BenchmarkWithArg<int>(const char* name, void (*fn)(int, int), const char* arg_name, int arg) :
    BenchmarkWithArgBase<int>(name, fn, arg_name, arg) { }

  BenchmarkWithArg<int>* Arg(int arg) {
    char arg_name[100];
    PrettyPrintInt(arg_name, sizeof(arg_name), arg);
    BenchmarkRegister(new BenchmarkWithArg<int>(Name(), fn_arg_, arg_name, arg));
    return this;
  }
};

static inline Benchmark* BenchmarkFactory(const char* name, void (*fn)(int)) {
  return new Benchmark(name, fn);
}

template <typename T>
static inline BenchmarkWithArg<T>* BenchmarkFactory(const char* name, void (*fn)(int, T)) {
  return new BenchmarkWithArg<T>(name, fn);
}

}  // namespace testing

template <typename T>
static inline void BenchmarkAddArg(::testing::Benchmark* b, const char* name, T arg) {
  ::testing::BenchmarkWithArg<T>* ba;
  ba = static_cast< ::testing::BenchmarkWithArg<T>* >(b);
  ba->Arg(name, arg);
}

void SetBenchmarkBytesProcessed(int64_t);
void StopBenchmarkTiming();
void StartBenchmarkTiming();

#define BENCHMARK(f) \
    static ::testing::Benchmark* _benchmark_##f __attribute__((unused)) = \
        (::testing::Benchmark*)::testing::BenchmarkFactory(#f, f)

#endif // BIONIC_BENCHMARK_H_
