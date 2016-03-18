Malloc Debug
============

Malloc debug is a method of debugging native memory problems. It can help detect memory corruption, memory leaks, and use after free issues.

Currently, malloc debug requires root to enable. When it is enabled, it works by adding a shim layer that replaces the normal allocation calls. The replaced calls are:

<pre>
malloc
free
calloc
realloc
posix_memalign
memalign
malloc_usable_size
</pre>

On 32 bit systems, these two deprecated functions are also replaced:

<pre>
pvalloc
valloc
</pre>

Controlling Malloc Debug Behavior
---------------------------------
Malloc debug is controlled by individual options. Each option can be enable individually, or in a group of other options. Every single option can be combined with every other option.

Option Descriptions
-------------------
Below is a description of all of the different options:

<pre>
front_guard[=XX]
  Enables a front guard on all allocations. If XX is set
  it sets the number of bytes in the guard. The default is
  32 bytes, the max bytes is 16384.

rear_guard[=XX]
  Enables a rear guard on all allocations. If XX is set
  it sets the number of bytes in the guard. The default is
  32 bytes, the max bytes is 16384.

guard[=XX]
  Enables both a front guard and a rear guard on all allocations.
  If XX is set it sets the number of bytes in both guards.
  The default is 32 bytes, the max bytes is 16384.

backtrace[=XX]
  Enable capturing the backtrace at the point of allocation.
  If XX is set it sets the number of backtrace frames.
  The default is 16 frames, the max number of frames is 256.

backtrace_enable_on_signal[=XX]
  Enable capturing the backtrace at the point of allocation.
  The backtrace capture is not enabled until the process
  receives a signal. If XX is set it sets the number of backtrace
  frames. The default is 16 frames, the max number of frames is 256.

fill_on_alloc[=XX]
  On first allocation, fill with the value 0xeb.
  If XX is set it will only fill up to XX bytes of the
  allocation. The default is to fill the entire allocation.

fill_on_free[=XX]
  On free, fill with the value 0xef. If XX is set it will
  only fill up to XX bytes of the allocation. The default is to
  fill the entire allocation.

fill[=XX]
  On both first allocation free, fill with the value 0xeb on
  first allocation and the value 0xef. If XX is set, only fill
  up to XX bytes. The default is to fill the entire allocation.

expand_alloc[=XX]
  Allocate an extra number of bytes for every allocation call.
  If XX is set, that is the number of bytes to expand the
  allocation by. The default is 16 bytes, the max bytes is 16384.

free_track[=XX]
  When a pointer is freed, do not free the memory right away.
  Instead, keep XX of these allocations around and then verify
  that they have not been modified when the total number of freed
  allocations exceeds the XX amount. When the program terminates,
  the rest of these allocations are verified. When this option is
  enabled, it automatically records the backtrace at the time of the free.
  The default is to record 100 allocations, the max allocations
  to record is 16384.

free_track_backtrace_num_frames[=XX]
  This option only has meaning if free\_track is set. This indicates
  how many backtrace frames to capture when an allocation is freed.
  If XX is set, that is the number of frames to capture. If XX
  is set to zero, then no backtrace will be captured.
  The default is to record 16 frames, the max number of frames is 256.

leak_track
  Enable the leak tracking of memory allocations.
</pre>

Tracking Memory Leaks
---------------------
The other ability malloc debug has is the ability to track all of the currently live allocations and return them to a caller. In order to enable this tracking, enable either the backtrace or backtrace\_enabled\_on\_signal. The libc library on Android exports two calls to gather this data:

<pre>
"*info" is set to a buffer we allocate
"*overall_size" is set to the size of the "info" buffer
"*info_size" is set to the size of a single entry
"*total_memory" is set to the sum of all allocations we're tracking; does not include heap overhead
"*backtrace_size" is set to the maximum number of entries in the back trace
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall\_size, size_t* info_size, size_t* total_memory, size_t* backtrace_size);

Free the data allocated by a call to get_malloc_leak_info.
extern "C" void free_malloc_leak_info(uint8_t* info);
</pre>

If overall\_size is zero, then there are no allocations being tracked. The data buffer is a set of structures. The exact format is:

<pre>
size_t size_of_original_allocation
size_t num_backtrace_frames
uintptr_t pc1
uintptr_t pc2
uintptr_t pc3
.
.
.
</pre>

The number of uintptr_t values is determined by the value backtrace\_size as returned by the original call to get\_malloc\_leak\_info. This value is not variable, it is the same for all the returned data. The value num\_backtrace\_frames contains the real number of frames found. The extras frames are set to zero.
Each uintptr_t is a pc of the callstack. The calls from within the malloc debug library are automatically removed.
For 32 bit systems, size_t and uintptr_t are both 4 byte values.
For 64 bit systems, size_t and uintptr_t are both 8 byte values.
The total number of these structures returned is total\_backtrace\_allocs.
Once the data returned has been processed, then a call to free\_malloc\_leak\_info must be done to free the data allocated in the original call.

Examples
========
Enable backtrace tracking of all allocation for all processes:

<pre>
  adb shell stop
  adb shell setprop libc.debug.malloc.options backtrace
  adb shell start
</pre>

Enable backtrace tracking for a specific process (ls):

<pre>
  adb shell setprop libc.debug.malloc.program ls
  adb shell ls
</pre>

Enable backtrace tracking for the zygote and zygote based processes:

<pre>
  adb shell stop
  adb shell setprop libc.debug.malloc.program app\_process
  adb shell setprop libc.debug.malloc.options backtrace
  adb shell start
</pre>

Enable multiple options (backtrace and guards):

<pre>
  adb shell stop
  adb shell setprop libc.debug.malloc.options "backtrace guards"
  adb shell start
</pre>

Enable malloc debug when multiple processes have the same name. This method can be used to enable malloc debug for only a very specific process if multiple processes have the same name.

<pre>
  adb shell
  # setprop libc.debug.malloc.env\_enabled
  # setprop libc.debug.malloc.options backtrace
  # export LIBC\_DEBUG\_MALLOC\_ENABLE 1
  # ls
</pre>
