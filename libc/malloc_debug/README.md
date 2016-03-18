Malloc Debug
============

Malloc debug is a method of debugging native memory problems. It can help
detect memory corruption, memory leaks, and use after free issues.

Currently, malloc debug requires root to enable. When it is enabled, it works
by adding a shim layer that replaces the normal allocation calls. The replaced
calls are:

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

Any errors detected by the library are reported in the log.

Controlling Malloc Debug Behavior
---------------------------------
Malloc debug is controlled by individual options. Each option can be enabled
individually, or in a group of other options. Every single option can be
combined with every other option.

Option Descriptions
-------------------
### front\_guard[=XX]
Enables a small buffer placed before the allocated data. This is an attempt
to find memory corruption occuring to a region before the original allocation.
On first allocation, this front guard is written with a specific pattern (0xaa).
When the allocation is freed, the guard is checked to verify it has not been
modified. If any part of the front guard is modified, an error will be reported
in the log indicating what bytes changed.

If the backtrace option is also enabled, then any error message will include
the backtrace of the allocation creation.

If XX is present, it indicates the number of bytes in the guard. The default
is 32 bytes, the max bytes is 16384.

This option adds a special header to all allocations that contains the guard
and information about the original allocation.

Example error:

<pre>
+++ ALLOCATION 0x12345678 SIZE 100 HAS A CORRUPTED FRONT GUARD
  pointer[-32] = 0x00 (expected 0xaa)
  pointer[-15] = 0x02 (expected 0xaa)
</pre>

### rear\_guard[=XX]
Enables a small buffer placed after the allocated data. This is an attempt
to find memory corruption occuring to a region after the original allocation.
On first allocation, this rear guard is written with a specific pattern (0xbb).
When the allocation is freed, the guard is checked to verify it has not been
modified. If any part of the buffer is modified, an error will be reported
in the log indicating what bytes changed.

If XX is present, it indicates the number of bytes in the guard. The default
is 32 bytes, the max bytes is 16384.

This option adds a special header to all allocations that contains
information about the original allocation.

Example error:

<pre>
+++ ALLOCATION 0x12345678 SIZE 100 HAS A CORRUPTED REAR GUARD
  pointer[130] = 0xbf (expected 0xbb)
  pointer[131] = 0x00 (expected 0xbb)
</pre>

### guard[=XX]
Enables both a front guard and a rear guard on all allocations.

If XX is present, it indicates the number of bytes in both guards. The default
is 32 bytes, the max bytes is 16384.

### backtrace[=XX]
Enable capturing the backtrace at the point of allocation creation.
This option will slow down allocations by an order of magnitude. If the
system runs too slowly with this option enabled, decreasing the maximum number
of frames captured will speed the allocations up.

Note that any backtrace frames that occur within the malloc backtrace library
itself are not recorded.

If XX is present, it indicates the maximum number of frames to capture in
a backtrace. The default is 16 frames, the max number of frames is 256.

This option adds a special header to all allocations that contains the
backtrace and information about the original allocation.

### backtrace\_enable\_on\_signal[=XX]
Enable capturing the backtrace at the point of allocation creation. The
backtrace capture is toggled when the process receives a signal. When this
option is used alone, backtrace capture is start out disabled until the signal
is received. If both this option and the backtrace option are set, then
backtrace capture is enabled until the signal is received.

If XX is present, it sets the number of backtrace frames. The default is 16
frames, the max number of frames is 256.

This option adds a special header to all allocations that contains the
backtrace and information about the original allocation.

### fill\_on\_alloc[=XX]
Any allocation routine, other than calloc, will result in the allocation being
filled with the value 0xeb. When doing a realloc to a larger size, the bytes
above the original usable size will be set to 0xeb.

If XX is present, it will only fill up to XX bytes of the allocation. The default is to fill the entire allocation.

### fill\_on\_free[=XX]
When an allocation is freed, fill it with 0xef.

If XX is present, it will only fill up to XX bytes of the allocation. The
default is to fill the entire allocation.

### fill[=XX]
This enables both the fill\_on\_alloc option and the fill\_on\_free option.

If XX is present, only fill up to XX bytes of the allocation. The default
is to fill the entire allocation.

### expand\_alloc[=XX]
Add an extra amount to allocate for every allocation.

If XX is present, it is the number of bytes to expand the allocation by.
The default is 16 bytes, the max bytes is 16384.

### free\_track[=XX]
When a pointer is freed, do not free the memory right away, but add it to
a list of freed allocations. In addition to being added to the list, the
entire allocation is filled with the value 0xef, and the backtrace of
the free call is recorded.

When the list is full, an allocation is removed from the list and is
checked to make sure that none of the contents have been modified since
being placed on the list. When the program terminates, all of the allocations
left on the list are verified.

If XX is present, it indicates the total number of allocations in the list.
The default is to record 100 allocations, the max allocations to record is
16384.

This option adds a special header to all allocations that contains
information about the original allocation.

Example error:

<pre>
+++ ALLOCATION 0x12345678 USED AFTER FREE
  pointer[20] = 0xaf (expected 0xef)
  pointer[99] = 0x12 (expected 0xef)
Backtrace at time of free:
  #00 pc 0xfa
  #01 pc 0xeb
  #02 pc 0xdc
</pre>

In addition, there is another type of error message that can occur if
an allocation has a special header applied, and the header is corrupted
when the verification occurs. This is the error message that will be found
in the log:

<pre>
+++ ALLOCATION 0x12345678 HAS CORRUPTED HEADER TAG 0x1cc7dc00 AFTER FREE
</pre>

### free\_track\_backtrace\_num\_frames[=XX]
This option only has meaning if free\_track is set. It indicates how many
backtrace frames to capture when an allocation is freed.

If XX is present, it indicates the number of frames to capture. If the value
is set to zero, then no backtrace will be captured when the allocation is
freed. The default is to record 16 frames, the max number of frames to
to record is 256.

### leak\_track
Track all live allocations. When the program terminates, all of the live
allocations will be dumped to the log. If the backtrace option was enabled,
then the log will include the backtrace of the leaked allocations. This
options should be used sparingly since a lot of programs do not free everything
before the program terminates.

This option adds a special header to all allocations that contains
information about the original allocation.

Example leak error:

<pre>
+++ APP leaked block of size 1024 at 0x12345678 (leak 1 of 2)
Backtrace at time of allocation:
  #00 pc 0xfe000
  #01 pc 0xde000
+++ APP leaked block of size 100 at 0xa2345678 (leak 2 of 2)
Backtrace at time of allocation:
  #00 pc 0xfe000
  #01 pc 0xde000
</pre>

Additional Errors
-----------------
There are a few other error messages that might appear in the log.

### Use After Free
<pre>
+++ ALLOCATION 0x12345678 USED AFTER FREE (free)
Backtrace of original free:
  #00 pc 0xfa
  #01 pc 0xeb
  #02 pc 0xdc
Backtrace at time of failure:
  #00 pc 0x12
  #01 pc 0x22
  #02 pc 0x32
  #03 pc 0x42
</pre>

This indicates that code is attempting to free an already freed pointer. The
<i>(free)</i> indicates the function that was called when the problem was
found.

For example, this message:

<pre>
+++ ALLOCATION 0x12345678 USED AFTER FREE (realloc)
</pre>

Would indicate that the realloc was called with the already freed pointer.

### Invalid Tag
<pre>
+++ ALLOCATION 0x12345678 HAS INVALID TAG 1ee7d000 (malloc_usable_size)
Backtrace at time of failure:
  #00 pc 0xa
  #01 pc 0xb
  #02 pc 0xc
</pre>

This indicates that a function (malloc\_usable\_size) was called with
a pointer that is either not a real pointer allocated, or has been
corrupted.

As with the other error message, the function in parenthesis is the
function that was called with the bad pointer.

Native Memory Tracking
----------------------
Malloc debug can also be used to get information on all of the live allocations
in a process. The libc library in Android exports two calls that can be
used to gather this data from a process. This tracking can be enabled using
either the backtrace option or the backtrace\_enabled\_on\_signal option.

The function to gather the data:

<pre>
<b>
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size, size_t* total_memory, size_t* backtrace_size);
</b>
</pre>

<i>info</i> is set to a buffer allocated by the call that contains all of
the allocation information.
<i>overall\_size</i> is set to the total size of the buffer returned. If this
<i>info\_size</i>
value is zero, then there are no allocation being tracked.
<i>total\_memory</i> is set to the sum of all allocation sizes that are live at
the point of the function call. This does not include the memory allocated
by the malloc debug library itself.
<i>backtrace\_size</i> is set to the maximum number of backtrace entries
that are present for each allocation.

In order to free the buffer allocated by the function, call:

<pre>
<b>
extern "C" void free_malloc_leak_info(uint8_t* info);
</b>
</pre>

### Format of info Buffer
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

The number of <i>uintptr\_t</i> values is determined by the value
<i>backtrace\_size</i> as returned by the original call to
<i>get\_malloc\_leak\_info</i>. This value is not variable, it is the same
for all the returned data. The value
<i>num\_backtrace\_frames</i> contains the real number of frames found. The
extra frames are set to zero. Each <i>uintptr\_t</i> is a pc of the callstack.
The calls from within the malloc debug library are automatically removed.

For 32 bit systems, <i>size\_t</i> and <i>uintptr\_t</i> are both 4 byte values.

For 64 bit systems, <i>size\_t</i> and <i>uintptr\_t</i> are both 8 byte values.

The total number of these structures returned in <i>info</i> is
<i>overall\_size</i> divided by <i>info\_size</i>.

Note, the size value in each allocation data structure will have bit 31 set
if this allocation was created by the Zygote process. This helps to distinguish
between native allocations created by the application.

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

Enable malloc debug when multiple processes have the same name. This method
can be used to enable malloc debug for only a very specific process if
multiple processes have the same name.

<pre>
  adb shell
  # setprop libc.debug.malloc.env\_enabled
  # setprop libc.debug.malloc.options backtrace
  # export LIBC\_DEBUG\_MALLOC\_ENABLE 1
  # ls
</pre>
