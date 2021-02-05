# This file added for experimental interoperability with Bazel.
package(
    default_visibility = ["@//:__pkg__"],
)

genrule(
    name = "syscalls-arm",
    srcs = ["SYSCALLS.TXT"],
    outs = ["syscalls-arm.S"],
    cmd = "$(location //bionic/libc/tools:bionic-gensyscalls) arm $< > $@",
    tools = ["//bionic/libc/tools:bionic-gensyscalls"],
)

genrule(
    name = "syscalls-arm64",
    srcs = ["SYSCALLS.TXT"],
    outs = ["syscalls-arm64.S"],
    cmd = "$(location //bionic/libc/tools:bionic-gensyscalls) arm64 $< > $@",
    tools = ["//bionic/libc/tools:bionic-gensyscalls"],
)

genrule(
    name = "syscalls-x86",
    srcs = ["SYSCALLS.TXT"],
    outs = ["syscalls-x86.S"],
    cmd = "$(location //bionic/libc/tools:bionic-gensyscalls) x86 $< > $@",
    tools = ["//bionic/libc/tools:bionic-gensyscalls"],
)

genrule(
    name = "syscalls-x86_64",
    srcs = ["SYSCALLS.TXT"],
    outs = ["syscalls-x86_64.S"],
    cmd = "$(location //bionic/libc/tools:bionic-gensyscalls) x86_64 $< > $@",
    tools = ["//bionic/libc/tools:bionic-gensyscalls"],
)
