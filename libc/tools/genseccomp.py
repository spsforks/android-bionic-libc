#!/usr/bin/python
import os
import sys
from subprocess import Popen, PIPE
from gensyscalls import SysCallsTxtParser

def CreateOneFile(architecture, syscall_file, header_dir, output_file):
    parser = SysCallsTxtParser()
    parser.parse_file(syscall_file)
    syscalls = parser.syscalls

    # Select only elements where the last string contains the specified architecture or any
    syscalls = filter(lambda x: architecture in x and x[architecture], syscalls)

    # We only want the name
    content = map(lambda x: x["name"], syscalls)

    # Run cpp over the __NR_syscall symbols, including unistd.h, to get the actual numbers
    prefix = "__SECCOMP_" # prefix to (almost) ensure no name collisions
    cpp = Popen(['cpp', '-I'  + header_dir], stdin=PIPE, stdout=PIPE)
    cpp.stdin.write("#include <unistd.h>\n")
    for i in content:
        if "_NR_" not in i: # Kludge to cope with arm specific functions
            cpp.stdin.write(prefix + i + ", __NR_" + i + "\n")
        else:
            cpp.stdin.write(prefix + i + ", " + i + "\n")
    cpp.stdin.close()

    # Read in the preprocessed result
    content = [l.strip('\n') for l in cpp.stdout.readlines()]

    # Extract only the lines with our unique symbols
    content = [l for l in content if l[0:len(prefix)] == prefix]

    # Split on the commas we put into the source file
    content = [[w.strip() for w in l.split(',')] for l in content]

    # Remove the prefix from the name, and convert the number into a string
    # Note that some of the numbers wer expressed as base + offset, so we need to eval, not just int
    content = [[l[0][len(prefix):], eval(l[1])] for l in content]

    # Sort the values so we convert to ranges and binary chop
    content = sorted(content, lambda x,y: cmp(x[1], y[1]))

    # Turn into a set of ranges. Keep the names for the comments
    ranges = []
    for i in content:
        if ranges and ranges[-1][1] == i[1]:
            ranges[-1][1] = i[1] + 1
            ranges[-1][2].append(i[0])
        else:
            ranges.append([i[1], i[1] + 1, [i[0]]])

    # Chop handles a range of range - either further chop, or if of size 1 output BPFs
    def Chop(ranges):
        if len(ranges) == 1:
            # We will replace fail and allow with appropriate range jumps later
            return ["BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, " + str(ranges[0][1]) + ", fail else allow), //" + "|".join(ranges[0][2])]
        else:
            half = (len(ranges) + 1) / 2
            first = Chop(ranges[:half])
            second = Chop(ranges[half:])
            return ["BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, " + str(ranges[half][0]) + ", " + str(len(first)) + ", 0),"] + first + second

    # Binary chop the ranges!
    output = Chop(ranges)

    # Now we know the size of the tree, we can substitute the "fail else allow" statements
    for i in range(0, len(output)):
        if "fail else allow" in output[i]:
            jumps = str(len(output) - i - 1) + ", " + str(len(output) - i);
            output[i] = output[i].replace("fail else allow", jumps)

    # Add check that we aren't off the bottom of the syscalls
    output.insert(0, "BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, " + str(ranges[0][0]) + ", 0, " + str(len(output)) + "),")

    # Add the error and allow calls at the end
    output.append("BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_KILL),")
    output.append("BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),")

    # And output policy
    output = [
    "// Autogenerated file - edit at your peril!!",
    "",
    "#include <linux/filter.h>",
    "#include <errno.h>",
    "",
    "#include \"seccomp_policy.h\""
    "",
    "const struct sock_filter " + architecture + "_filter[] = {",
    ] + output + [
    "};",
    "",
    "const size_t " + architecture + "_filter_size = sizeof(" + architecture + "_filter) / sizeof(struct sock_filter);"
    ]

    if os.path.isfile(output_file) and output == [x.strip('\n') for x in open(output_file).readlines()]:
        print "File " + output_file + " not changed."
    else:
        open(output_file, 'w').writelines("%s\n" % item for item in output)
        print "Generated file " + output_file

# Set working directory for predictable results
os.chdir(os.path.join(os.environ["ANDROID_BUILD_TOP"], "bionic/libc"))
syscall_file = "SYSCALLS.TXT"
CreateOneFile("arm", syscall_file, "kernel/uapi/asm-arm/asm", "seccomp/arm_policy.c")
CreateOneFile("arm64", syscall_file, "kernel/uapi/asm-arm64/asm", "seccomp/arm64_policy.c")

