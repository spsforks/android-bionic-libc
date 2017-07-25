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

#include <getopt.h>
#include <math.h>
#include <sys/resource.h>

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>
#include <tinyxml2.h>
#include "util.h"

std::map<std::string, benchmark_func_t> str_to_func;

std::mutex map_lock;

static struct option long_options[] =
{
  {"bionic_cpu", required_argument, 0, 'c'},
  {"bionic_xml", required_argument, 0, 'x'},
  {"bionic_iterations", required_argument, 0, 'i'},
  {"bionic_extra", required_argument, 0, 'a'},
  {"help", no_argument, 0, 'h'},
  {0, 0, 0, 0},
};

void usage() {
  printf("Usage:\n");
  printf("bionic_benchmarks [--bionic_cpu=<cpu_to_isolate>] [--bionic_xml=<path_to_xml>]\n");
  printf("                  [--bionic_iterations=<num_iter>]\n");
  printf("                  [--bionic_extra=\"<fn_name> <arg1> <arg 2> ... \n]");
  printf("                  [<Google benchmark flags>]\n");
  printf("Google benchmark flags:\n");

  int fake_argc = 2;
  char argv0[] = "bionic_benchmarks";
  char argv1[] = "--help";
  char* fake_argv[3] {argv0, argv1, NULL};
  benchmark::Initialize(&fake_argc, fake_argv);
  abort();
}

void SanitizeOpts(int* argc, char** argv, int* new_argc, char** new_argv) {
  // TO THOSE ADDING OPTIONS: This currently doesn't support optional arguments,
  *new_argc = 1;
  new_argv[0] = argv[0];
  for (int i = 1; i < *argc; ++i) {
    char* optarg = argv[i];

    int opt_idx = 0;
    for (opt_idx = 0; long_options[opt_idx].name &&
                           strncmp(long_options[opt_idx].name, optarg + 2,
                           strlen(long_options[opt_idx].name)); ++opt_idx) {
    }

    if (!long_options[opt_idx].name) {
      new_argv[*new_argc] = optarg;
      *new_argc += 1;
    } else {
      if (long_options[opt_idx].has_arg == required_argument) {
        // If the arg was passed in with an =, it spans one char *.
        // Otherwise, we skip a spot for the argument.
        if (!strchr(optarg, '=')) {
          i++;
        }
      }
    }
  }
  new_argv[*new_argc] = 0;
}

bench_opts_t parse_opts(int argc, char** argv) {
  bench_opts_t opts;
  int opt;
  int option_index = 0;

  opts.cpu_to_lock = INT_MAX;
  opts.num_iterations = 0;
  opts.xmlpath = "";

  // To make this parser handle the benchmark options silently:
  extern int opterr;
  opterr = 0;

  while ((opt = getopt_long(argc, argv, "c:x:i:a:h", long_options, &option_index)) != -1) {
    if (opt == -1) {
      break;
    }
    switch (opt) {
      case 'c':
        if (*optarg) {
          char *check_null;
          opts.cpu_to_lock = static_cast<int>(strtol(optarg, &check_null, 10));
          if (*check_null) {
            printf("ERROR: Args %s is not a valid integer.\n", optarg);
            abort();
          }
        } else {
          printf("ERROR: no argument specified for bionic_cpu\n");
          usage();
        }
        break;
      case 'x':
        if (*optarg) {
          opts.xmlpath = std::string(optarg);
        } else {
          printf("ERROR: no argument specified for bionic_xml\n");
          usage();
        }
        break;
      case 'a':
        if (*optarg) {
          opts.extra_benchmarks.push_back(std::string(optarg));
        } else {
          printf("ERROR: no argument specified for bionic_extra\n");
          usage();
        }
        break;
      case 'i':
        if (*optarg){
          char *check_null;
          opts.num_iterations = static_cast<int>(strtol(optarg, &check_null, 10));
          if (*check_null) {
            printf("ERROR: Args %s is not a valid integer.\n", optarg);
            abort();
          }
        } else {
          printf("ERROR: no argument specified for bionic_iterations\n");
          usage();
        }
        break;
      case 'h':
        usage();
        break;
      case '?':
        break;
      default:
        abort();
    }
  }
  return opts;
}

// This is a wrapper for every function call for per-benchmark cpu pinning.
void LockAndRun(benchmark::State& state, benchmark_func_t func_to_bench, int cpu_to_lock) {
  if (cpu_to_lock != INT_MAX)
    LockToCPU(cpu_to_lock);
  // To avoid having to link against Google benchmarks in libutil,
  // benchmarks are kept without parameter information, necessitating this cast.
  reinterpret_cast<void(*) (benchmark::State&)>(func_to_bench)(state);
}

std::vector<std::vector<int>>* ResolveArgs(std::vector<std::vector<int>>* rv, std::string args) {
  // args is either a space-separated list of ints or a macro name.
  // To ease formatting in XML files, args is left and right trimmed.
  if (args_shorthand.count(args)) {
    return &args_shorthand[args];
  } else {
    rv->push_back(std::vector<int>());
    std::stringstream sstream(args);
    std::string argstr;
    while (sstream >> argstr) {
      char *check_null;
      int converted = static_cast<int>(strtol(argstr.c_str(), &check_null, 10));
      if (*check_null) {
        printf("ERROR: Args str %s contains an invalid macro or int.\n", args.c_str());
        abort();
      }
      (*rv)[0].push_back(converted);
    }
    return rv;
  }
}

void RegisterGoogleBenchmarks(bench_opts_t primary_opts, bench_opts_t secondary_opts,
                         std::string fn_name, std::vector<std::vector<int>> *run_args,
                         const std::map<std::string, benchmark_func_t> &str_to_func) {
  if (str_to_func.find(fn_name) == str_to_func.end()) {
    printf("ERROR:No benchmark for function %s\n", fn_name.c_str());
    abort();
  }
  int iterations_to_use = primary_opts.num_iterations ? primary_opts.num_iterations : \
                                                    secondary_opts.num_iterations;
  int cpu_to_use = INT_MAX;
  if (primary_opts.cpu_to_lock != INT_MAX) {
    cpu_to_use = primary_opts.cpu_to_lock;

  } else if (secondary_opts.cpu_to_lock != INT_MAX) {
    cpu_to_use = secondary_opts.cpu_to_lock;
  }

  for (std::vector<int> args : (*run_args)) {
    if (iterations_to_use > 0) {
      benchmark::RegisterBenchmark(fn_name.c_str(), LockAndRun, str_to_func.at(fn_name),
                                   cpu_to_use)->Args(args)->Iterations(iterations_to_use);
    } else {
      benchmark::RegisterBenchmark(fn_name.c_str(), LockAndRun, str_to_func.at(fn_name),
                                   cpu_to_use)->Args(args);
    }
  }
}

void RegisterCliBenchmarks(bench_opts_t cmdline_opts,
                           const std::map<std::string, benchmark_func_t> &str_to_func) {
  // Register any of the extra benchmarks that were specified in the options.
  std::vector<std::vector<int>> arg_vector;
  std::vector<std::vector<int>> *run_args = &arg_vector;
  for (std::string extra_fn : cmdline_opts.extra_benchmarks) {
    trim(extra_fn);
    int first_space_pos = extra_fn.find(" ");
    std::string fn_name = extra_fn.substr(0, extra_fn.find(" "));
    std::string cmd_args;
    if (first_space_pos > 0) {
      cmd_args = extra_fn.substr(extra_fn.find(" ") + 1);
    } else {
      cmd_args = "";
    }
    run_args = ResolveArgs(run_args, cmd_args);
    RegisterGoogleBenchmarks(bench_opts_t(), cmdline_opts, fn_name, run_args, str_to_func);

    run_args = &arg_vector;
    arg_vector.clear();
  }
}

int RegisterXmlBenchmarks(bench_opts_t cmdline_opts,
                        const std::map<std::string, benchmark_func_t> &str_to_func) {
  // Structure of the XML file:
  // - Element "fn"           Function to benchmark.
  // - - Element "iterations" Number of iterations to run. Leaving this blank uses
  //                          Google benchmarks' convergence heuristics.
  // - - Element "cpu"        CPU to isolate to, if any.
  // - - Element "args"       Whitespace-separated list of per-function integer arguments, or
  //                          one of the macros defined in util.h.
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(cmdline_opts.xmlpath.c_str()) != tinyxml2::XML_NO_ERROR) {
    doc.PrintError();
    return doc.ErrorID();
  }

  // Read and register the functions.
  tinyxml2::XMLNode* fn = doc.FirstChildElement("fn");
  std::vector<std::vector<int>> arg_vector;
  std::vector<std::vector<int>> *run_args = &arg_vector;
  while (fn) {
    if (!fn) {
      printf("Error: Malformed XML entry: no top-layer fn element.\n");
      abort();
    }
    auto fn_elem = fn->FirstChildElement("name");
    if (!fn_elem) {
      printf("Error: Malformed XML entry: missing name element.\n");
      abort();
    }
    std::string fn_name = fn_elem->GetText();
    if (fn_name.empty()) {
      printf("Error: Malformed XML entry: error parsing name text.");
      abort();
    }
    auto* xml_args = fn->FirstChildElement("args");
    run_args = ResolveArgs(run_args, xml_args ? trim(xml_args->GetText()) : std::string());

    // XML values for CPU and iterations take precedence over those passed in via CLI.
    bench_opts_t xml_opts{};
    auto* num_iterations_elem = fn->FirstChildElement("iterations");
    if (num_iterations_elem) {
      num_iterations_elem->QueryIntText(&xml_opts.num_iterations);
    } else {
      xml_opts.num_iterations = 0;
    }
    auto* cpu_to_lock_elem = fn->FirstChildElement("cpu");
    if (cpu_to_lock_elem) {
      cpu_to_lock_elem->QueryIntText(&xml_opts.cpu_to_lock);
    } else {
      xml_opts.cpu_to_lock = INT_MAX;
    }

    RegisterGoogleBenchmarks(xml_opts, cmdline_opts, fn_name, run_args, str_to_func);

    fn = fn->NextSibling();
    run_args = &arg_vector;
    arg_vector.clear();
  }
  return 0;
}

int main(int argc, char** argv) {
  int new_argc;
  for (int i = 1; i < 15; i++) {
    int align = pow(2, i);
    std::stringstream sstream;
    sstream << "AT_" << align << "_ALIGN_TWOBUF";
    args_shorthand.emplace(sstream.str(),
                           std::vector<std::vector<int>>{ {8, align, align},
                                                          {64, align, align},
                                                          {512, align, align},
                                                          {1 * KB, align, align},
                                                          {8 * KB, align, align},
                                                          {16 * KB, align, align},
                                                          {32 * KB, align, align},
                                                          {64 * KB, align, align} });
    sstream.str("");
    sstream << "AT_" << align << "_ALIGN_ONEBUF";
    args_shorthand.emplace(sstream.str(),
                            std::vector<std::vector<int>>{ {(8), align},
                                                           {(64), align},
                                                           {(512), align},
                                                           {(1*KB), align},
                                                           {(8*KB), align},
                                                           {(16*KB), align},
                                                           {(32*KB), align},
                                                           {(64*KB), align} });
    sstream.str("");
  }
  bench_opts_t opts = parse_opts(argc, argv);
  std::vector<char*> new_argv(argc);
  SanitizeOpts(&argc, argv, &new_argc, new_argv.data());

  if (!opts.xmlpath.empty()) {
    if (int err = RegisterXmlBenchmarks(opts, str_to_func)) {
      return err;
    }
  }
  RegisterCliBenchmarks(opts, str_to_func);

  // Set the thread priority to the maximum.
  if (setpriority(PRIO_PROCESS, 0, -20)) {
      perror("Failed to raise priority of process. Are you root?\n");
  }
  benchmark::Initialize(&new_argc, new_argv.data());
  benchmark::RunSpecifiedBenchmarks();
}
