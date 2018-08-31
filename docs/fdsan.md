## fdsan

[TOC]

### Background
*What problem is fdsan trying to solve? Why should I care?*

fdsan (file descriptor sanitizer) detects mishandling of file descriptor ownership, which tend to manifest as *use-after-close* and *double-close*. These errors are direct analogues of the memory allocation *use-after-free* and *double-free* bugs, but tend to be much more difficult to diagnose and fix. With `malloc` and `free`, implementations have free reign to detect errors and abort on double free. File descriptors, on the other hand, are mandated by the C standard to be allocated with the lowest available number being returned for new allocations. As a result, many file descriptor bugs can *never* be noticed on the thread on which the error occurred, and will manifest as "impossible" behavior on another thread.

For example, given two threads running the following code:
```cpp
void thread_one() {
    int fd = open("/dev/null", O_RDONLY);
    close(fd);
    close(fd);
}

void thread_two() {
    while (true) {
        int fd = open("log", O_WRONLY | O_APPEND);
        if (write(fd, "foo", 3) != 3) {
            err(1, "write failed!");
        }
    }
}
```
the following interleaving is possible:
```cpp
thread one                                thread two
open("/dev/null", O_RDONLY) = 123
close(123) = 0
                                          open("log", O_WRONLY | APPEND) = 123
close(123) = 0
                                          write(123, "foo", 3) = -1 (EBADF)
                                          err(1, "write failed!")
```

Assertion failures are probably the most innocuous result that can arise from these bugs: silent data corruption [[1](#footnote1), [2](#footnote2)] or security vulnerabilities are also possible (e.g. suppose thread two was saving user data to disk when a third thread came in and opened a socket to the Internet).

### Design
*What does fdsan do?*

fdsan attempts to detect and/or prevent file descriptor mismanagement by enforcing file descriptor ownership. Like how most memory allocations can have their ownership handled by types such as `std::unique_ptr`, almost all file descriptors can be associated with a unique owner which is responsible for their closure. fdsan provides functions to associate a file descriptor with an owner; if someone tries to close a file descriptor that they don't own, depending on configuration, either a warning is emitted, or the process aborts.

The way this is implemented is by providing functions to set a 64-bit closure tag on a file descriptor. The tag consists of an 8-bit type byte that identifies the type of the owner (`enum android_fdan_owner_type` in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h)), and a 56-bit value. The value should ideally be something that uniquely identifies the object (object address for native objects and `System.identityHashCode` for Java objects), but in cases where it's hard to derive an identifier for the "owner" that should close a file descriptor, even using the same value for all file descriptors in the module can be useful, since it'll catch other code that closes your file descriptors.

If a file descriptor that's been marked with a tag is closed with an incorrect tag, or without a tag, we know something has gone wrong, and can generate diagnostics or abort.

### Enabling fdsan (as a user)
*How do I use fdsan?*

fdsan has four severity levels:
 - disabled (`ANDROID_FDSAN_ERROR_LEVEL_DISABLED`)
 - warn-once (`ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE`)
   - Upon detecting an error, emit a warning to logcat, generate a tombstone, and then continue execution with fdsan disabled.
 - warn-always (`ANDROID_FDSAN_ERROR_LEVEL_WARN_ALWAYS`)
   - Same as warn-once, except without disabling after the first warning.
 - fatal (`ANDROID_FDSAN_ERROR_LEVEL_FATAL`)
   - Abort upon detecting an error.

In Android Q, fdsan has a global default of warn-once. fdsan can be made more or less strict at runtime via the `android_fdsan_set_error_level` function in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h).

The likelihood of fdsan catching a file descriptor error is proportional to the percentage of file descriptors in your process that are tagged with an owner.

### Using fdsan to fix a bug
*No, really, how do I use fdsan?*

Often, fdsan will seem to indicate an error in code that seems to be correct.

For example, the following code which is obviously correct:
```cpp
int wat() {
  int fd = open("/dev/null", O_RDONLY);
  sleep(1);
  close(fd);
}
```
might get blamed by fdsan:
```
pid: 12345, tid: 12345, name: foo  >>> foo <<<
signal 35 (<debuggerd signal>), code -1 (SI_QUEUE), fault addr --------
Abort message: 'attempted to close file descriptor fd 42, expected to be unowned, actually owned by ParcelFileDescriptor 0x629617'
    x0  0000000000000000  x1  0000000000004c36  x2  0000000000000023  x3  0000007dd794e238
    x4  0000007dd794e4c4  x5  786520736177202c  x6  786520736177202c  x7  7420646574636570
    x8  00000000000000f0  x9  0000000000000001  x10 0000000000000000  x11 0000007e759169b7
    x12 0000007e7591696b  x13 64656e776f6e7520  x14 0000007dd794e216  x15 0000000000000000
    x16 0000007e7595a0f0  x17 0000007e75882a40  x18 0000000000000000  x19 0000007dd7950598
    x20 0000000000000023  x21 0000007dd794e440  x22 0000000000004c36  x23 0000007dd7950588
    x24 0000000000000000  x25 0000007dd7950588  x26 0000007deb4234a0  x27 0000000000000000
    x28 0000000000000000  x29 0000007dd794e2f0
    sp  0000007dd794e230  lr  0000007e758e7a64  pc  0000007e75882a60

backtrace:
    #00 pc 000000000000894c  /system/lib64/libc.so (fdsan_error(char const*, ...)+284)
    #01 pc 0000000000001234  /data/local/tmp/foo (wat()+32)
```
The reason for this is (hopefully!) not a bug in fdsan. What actually happened is that in between `wat`'s open and close, the culprit closed `wat`'s file descriptor and a third thread happened to create a `ParcelFileDescriptor` that happened to land on `wat`'s file descriptor number. Changing `wat` to use an fdsan-enabled type as follows:

```cpp
int wat() {
  android::base::unique_fd fd(open("/dev/null", O_RDONLY));
  sleep(1);
  fd.reset();
}
```
will result in a much more useful backtrace that points to the offender closing `wat`'s `unique_fd`.


### Enabling fdsan (as a C++ library implementer)

fdsan operates via two main primitives. `android_fdsan_exchange_owner_tag` modifies a file descriptor's close tag, and `android_fdsan_close_with_tag` closes a file descriptor with its tag. In the `<android/fdsan.h>` header, these are marked with `__attribute__((weak))`, so instead of passing down the platform version from JNI, availability of the functions can be queried directly. An example implementation of unique_fd follows:

```cpp
/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <android/fdsan.h>
#include <unistd.h>

#include <utility>

struct unique_fd {
    unique_fd() = default;

    explicit unique_fd(int fd) {
        reset(fd);
    }

    unique_fd(const unique_fd& copy) = delete;
    unique_fd(unique_fd&& move) {
        *this = std::move(move);
    }

    ~unique_fd() {
        reset();
    }

    unique_fd& operator=(const unique_fd& copy) = delete;
    unique_fd& operator=(unique_fd&& move) {
        if (this == &move) {
            return *this;
        }

        reset();

        if (move.fd_ != -1) {
            fd_ = move.fd_;
            move.fd_ = -1;

            // Acquire ownership from the moved-from object.
            exchange_tag(fd_, move.tag(), tag());
        }

        return *this;
    }

    int get() { return fd_; }

    int release() {
        if (fd_ == -1) {
            return -1;
        }

        int fd = fd_;
        fd_ = -1;

        // Release ownership.
        exchange_tag(fd, tag(), 0);
        return fd;
    }

    void reset(int new_fd = -1) {
        if (fd_ != -1) {
            close(fd_, tag());
            fd_ = -1;
        }

        if (new_fd != -1) {
            fd_ = new_fd;

            // Acquire ownership of the presumably unowned fd.
            exchange_tag(fd_, 0, tag());
        }
    }

  private:
    int fd_ = -1;

    // The obvious choice of tag to use is the address of the object.
    uint64_t tag() {
        return reinterpret_cast<uint64_t>(this);
    }

    // These functions are marked with __attribute__((weak)), so that their
    // availability can be determined at runtime. These wrappers will use them
    // if available, and fall back to no-ops or regular close on pre-Q devices.
    static void exchange_tag(int fd, uint64_t old_tag, uint64_t new_tag) {
        if (android_fdsan_exchange_owner_tag) {
            android_fdsan_exchange_owner_tag(fd, old_tag, new_tag);
        }
    }

    static int close(int fd, uint64_t tag) {
        if (android_fdsan_close_with_tag) {
            return android_fdsan_close_with_tag(fd, tag);
        } else {
            return ::close(fd);
        }
    }
};
```

### Frequently seen bugs
 * Native APIs not making it clear when they take ownership of a file descriptor. <br/>
   * Solution: accept `unique_fd` instead of `int` in functions that take ownership.
   * [Example one](https://android-review.googlesource.com/c/platform/system/core/+/721985), [two](https://android-review.googlesource.com/c/platform/frameworks/native/+/709451)
 * Receiving a `ParcelFileDescriptor` via Intent, and then passing it into JNI code that ends up calling close on it. <br/>
   * Solution: ¯\\\_(ツ)\_/¯. Use fdsan?
   * [Example one](https://android-review.googlesource.com/c/platform/system/bt/+/710104), [two](https://android-review.googlesource.com/c/platform/frameworks/base/+/732305)

### Footnotes
<a name="footnote1">1.</a> [How To Corrupt An SQLite Database File](https://www.sqlite.org/howtocorrupt.html#_continuing_to_use_a_file_descriptor_after_it_has_been_closed)

<a name="footnote2">2.</a> [<b><i>50%</i></b> of Facebook's iOS crashes caused by a file descriptor double close leading to SQLite database corruption](https://code.fb.com/ios/debugging-file-corruption-on-ios/)
