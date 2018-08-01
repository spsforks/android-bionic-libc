Validing libc Assembler Routines
================================
This document describes how to verify incoming assembler libc routines.

## Quick Start
==============
First, benchmark the previous version of the routine.
Update the routine, run the bionic unit tests to verify the routine doesn't
have any bugs.
See the [Testing](#Testing) section for details about how to verify that the
routine is being properly tested.
Rerun the benchmarks on the new routine and compare the results.
See the [Performance](#Performance) section for details about benchmarking.
When benchmarking, it's best to verify on the latest Pixel device supported.
Make sure that you benchmark both the big and little cores to verify that
there is no major difference in handling.

Benchmark 64 bit memcmp:

    /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks --bionic_xml=/data/benchmarktest64/bionic-benchmarks/suites/string.xml memcmp

Benchmark 32 bit memcmp:

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --bionic_xml=/data/benchmarktest/bionic-benchmarks/suites/string.xml memcmp

Locking to a specific cpu:

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks --bionic_cpu=2 --bionic_xml=/data/benchmarktest/bionic-benchmarks/suites/string.xml memcmp

## Performance
==============
The bionic benchmarks are used to verify the performance of changes to
routines. For most routines, there should already be benchmarks available.

Building
--------

    mmma -j bionic/benchmarks
    adb sync data

Running
-------
There are two bionic benchmarks executables:

    /data/benchmarktest64/bionic-benchmarks/bionic-benchmarks

This is for 64 bit libc routines.

    /data/benchmarktest/bionic-benchmarks/bionic-benchmarks

This is for 32 bit libc routines.

Here is an example of how the benchmark should be executed. For this
command to work, you need to change directory to one of the above
directories.

    bionic-benchmarks --bionic_xml=suites/string.xml memcmp

The last argument is the name of the one function that you want to
benchmark.

Almost all routines are already defined in the **string.xml** file in
**bionic/benchmarks/suites**. Look at the examples in that file to see
how to add a benchmark for a function that doesn't already exist.

It can take a long time to run these tests since it attempts to test a
bunch of sizes and alignments.

Results
-------
Bionic benchmarks is based on the [Google Benchmarks](https://github.com/google/benchmark)
library. An example of the output looks like this:

    Run on (8 X 1844 MHz CPU s)
    CPU Caches:
      L1 Data 32K (x8)
      L1 Instruction 32K (x8)
      L2 Unified 512K (x2)
    ***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
    -------------------------------------------------------------------------------------------
    Benchmark                                                    Time           CPU Iterations
    -------------------------------------------------------------------------------------------
    BM_string_memcmp/1/0/0                                       6 ns          6 ns  120776418   164.641MB/s
    BM_string_memcmp/1/1/1                                       6 ns          6 ns  120856788   164.651MB/s

The smaller the time, the better the performance.

Caveats
-------
When running the benchmarks, CPU scaling is not normally enabled. This means
that if the device does not get up to the maximum cpu frequency, the results
can vary wildly. It's possible to lock the cpu to the maximum frequency, but
is beyond the scope of this document. However, most of the benchmarks max
out the cpu very quickly on Pixel devices, and don't affect the results.

One problem is that the device can overheat when running the benchmarks. To
avoid this, you can run the device in a cool environment, or choose a
device that is less likely to overheat. To detect these kind of issues,
you can run an individual test and verify that it gets the same score as
the longer run.

## Testing
==========

Run the bionic tests to verify that the new routines are valid. However,
you should verify that there is coverage of the new routines. This is
especially important if this is the first time a routine is assembler.

Test Caveats
------------
When verifying an assembler routine that operates on buffer data (such as
memcpy/strcpy), it's important to verify these corner cases:

* Verify the routine does not read past the end of the buffers. Many
assembler routines optimize by reading multipe bytes at a time and can
read past the end and crash. This kind of bug results in 
* Verify the routine handles unaligned buffers properly. Usually, a failure
can result in an unaligned exception.
* Verify the routine handles different sized buffers.

If there are not sufficient tests for a new routine, there are a set of helper
functions that can be used to verify the above corner cases. See the
header bionic/tests/buffer\_tests.h for these routines and look at
bionic/tests/string\_test.cpp for examples of how to use it.
