### fdsan

[TOC]

#### Background
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
thread one                            thread two
open("/dev/null", O_RDONLY) = 123
close(123) = 0
                                      open("log", O_WRONLY | APPEND) = 123
close(123) = 0
                                      write(123, "foo", 3) = -1 (EBADF)
```

Assertion failures are probably the most innocuous result that can arise from these bugs: silent data corruption [[1](#footnote1), [2](#footnote2)] or security vulnerabilities are also possible (e.g. suppose thread two was saving user data to disk when a third thread came in and opened a socket to the Internet).

#### Design
*What does fdsan do?*

fdsan attempts to detect and/or prevent file descriptor mismanagement by enforcing file descriptor ownership. Almost all file descriptors can be associated with a unique owner which is responsible for their closure. fdsan provides functions to associate a file descriptor with an owner; if someone tries to close a file descriptor that they don't own, depending on configuration, either a warning is emitted, or the process aborts.

The way this is implemented is by providing functions to set a 64-bit closure tag on a file descriptor. The tag consists of an 8-bit type byte that identifies the type of the owner (`enum android_fdan_owner_type` in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h)), and a 56-bit value. The value should ideally be something that uniquely identifies the object (object address for native objects and `System.identityHashCode` for Java objects), but in cases where it's hard to derive an identifier for the "owner" that should close a file descriptor, even using the same value for all file descriptors in the module can be useful, since it'll catch other code from closing your file descriptors.

If a file descriptor that's been marked with a tag is closed with an incorrect tag, or without a tag, we know something has gone wrong, and can generate diagnostics or abort.

#### Enabling fdsan (as a user)
*How do I use fdsan?*

fdsan is enabled by default, in a warn-once mode. When fdsan detects an error, a backtrace is emitted to logcat, a tombstone is generated, and execution continues with fdsan disabled. fdsan can be made more or less strict at runtime via the `android_fdsan_set_error_level` function in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/master/libc/include/android/fdsan.h).

The likelihood of fdsan catching a file descriptor error is proportional to the percentage of file descriptors in your process that are tagged with an owner. To improve your odds, instead of keeping raw ints as members, use classes that maintain ownership such as `ParcelFileDescriptor` in Java, or `android::base::unique_fd` in C++ (see the library implementor section below if you're an NDK user).

#### Using fdsan to fix a bug
*No, really, how do I use fdsan?*

Often, fdsan will generate an error in code that seems to be correct.

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

#### Enabling fdsan (as a library implementer)
An example implementation of a C++ class that utilizes fdsan can be seen at <font color="red">TODO</font>. If you're using Java, just wrap a ParcelFileDescriptor.

#### Frequently seen bugs
 * Native APIs not making it clear when they take ownership of a file descriptor. <br/>
   * Solution: accept `unique_fd` instead of `int` in functions that take ownership.
   * [Example one](https://android-review.googlesource.com/c/platform/system/core/+/721985), [two](https://android-review.googlesource.com/c/platform/frameworks/native/+/709451)
 * Receiving a `ParcelFileDescriptor` via Intent, and then passing it into JNI code that ends up calling close on it. <br/>
   * Solution: ¯\\_(ツ)_/¯. Use fdsan?
   * [Example one](https://android-review.googlesource.com/c/platform/system/bt/+/710104), [two](https://android-review.googlesource.com/c/platform/frameworks/base/+/732305)

#### Footnotes
<a name="footnote1">1.</a> [How To Corrupt An SQLite Database File](https://www.sqlite.org/howtocorrupt.html#_continuing_to_use_a_file_descriptor_after_it_has_been_closed)

<a name="footnote2">2.</a> [<b><i>50%</i></b> of Facebook's iOS crashes caused by a file descriptor double close leading to SQLite database corruption](https://code.fb.com/ios/debugging-file-corruption-on-ios/)
