#!/usr/bin/env python
import collections
import os
import re
import sys
import textwrap
from gensyscalls import SysCallsTxtParser
from subprocess import Popen, PIPE


BPF_JGE = "BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, {0}, {1}, {2})"
BPF_ALLOW = "BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW)"


class SyscallRange(object):
  def __init__(self, name, value):
    self.names = [name]
    self.begin = value
    self.end = self.begin + 1

  def __str__(self):
    return "(%s, %s, %s)" % (self.begin, self.end, self.names)

  def add(self, name, value):
    if value != self.end:
      raise ValueError
    self.end += 1
    self.names.append(name)


def get_allowed_names(syscall_files, architecture):
  syscall_lists = []
  for syscall_file in syscall_files:
    parser = SysCallsTxtParser()
    parser.parse_open_file(syscall_file)
    syscall_lists.append(parser.syscalls)

  bionic, whitelist, blacklist = syscall_lists[0], syscall_lists[1], syscall_lists[2]
  for x in blacklist:
    if not x in bionic:
      raise RuntimeError("Blacklist item not in bionic - aborting " + str(x))

    if x in whitelist:
      raise RuntimeError("Blacklist item in whitelist - aborting " + str(x))

  bionic_minus_blacklist = [x for x in bionic if x not in blacklist]
  syscalls = bionic_minus_blacklist + whitelist

  # Select only elements matching required architecture
  syscalls = [x for x in syscalls if architecture in x and x[architecture]]

  # We only want the name
  names = [x["name"] for x in syscalls]

  # Check for duplicates
  dups = [name for name, count in collections.Counter(names).items() if count > 1]

  # x86 has duplicate socketcall entries, so hard code for this
  if architecture == "x86":
    dups.remove("socketcall")

  if len(dups) > 0:
    raise RuntimeError("Duplicate entries found - aborting " + str(dups))

  # Remove remaining duplicates
  return list(set(names))


def expand_defs(definitions):
  # The __NR macro definitions take one of 3 forms:
  #   #define __NR_foo 123
  #   #define __NR_bar __NR_foo
  #   #define __NR_baz (__NR_foo + 456)
  result = dict()

  for name, value in definitions.copy().items():
    try:
      if value.startswith("0x"):
        result[name] = int(value, 16)
      else:
        result[name] = int(value)
      del definitions[name]
    except ValueError:
      continue

  # Do this in a loop, to incrementally resolve dependencies
  defs_left = len(definitions)
  identical = re.compile(r"([_a-zA-Z0-9]+)$")
  addition = re.compile(r"\(([_a-zA-Z0-9]+) \+ (0x[0-9a-fA-F]+|\d+)\)$")
  while defs_left != 0:
    for name, value in definitions.copy().items():
      ident_match = identical.match(value)
      if ident_match:
        other = ident_match.groups()[0]
        if other in result:
          result[name] = result[other]
          del definitions[name]
        continue

      addition_match = addition.match(value)
      if addition_match:
        other = addition_match.groups()[0]
        imm = addition_match.groups()[1]
        if imm.startswith("0x"):
          imm = int(imm, 16)
        else:
          imm = int(imm)
        if other in result:
          result[name] = result[other] + imm
          del definitions[name]
        continue

      raise RuntimeError("unmatched definition: '{}' => '{}'".format(name, value))

    if defs_left != 0 and defs_left == len(definitions):
      raise RuntimeError("made no progress")

    defs_left = len(definitions)

  return result


def get_syscalls(header_dir, extra_switches):
  cpp = Popen(["../../prebuilts/clang/host/linux-x86/clang-stable/bin/clang",
               "-dM", "-E", "-nostdinc", "-I" + header_dir, "-Ikernel/uapi/"]
               + extra_switches
               + ["-"],
              stdin=PIPE, stdout=PIPE)
  cpp.stdin.write("#include <asm/unistd.h>\n")

  syscalls = dict()
  for line in cpp.communicate()[0].split("\n"):
    if not line.startswith("#define "):
      continue
    (name, value) = line[len("#define "):].split(" ", 1)

    if "_NR" in name:
      syscalls[name] = value

  return expand_defs(syscalls)


def convert_names_to_NRs(syscalls, names):
  result = []
  for name in names:
    if name in syscalls:
      result.append((name, syscalls[name]))
    elif ("__NR_" + name) in syscalls:
      result.append((name, syscalls["__NR_" + name]))
    else:
      raise RuntimeError("unknown name '{}'".format(name))
  return result


def convert_NRs_to_ranges(syscalls):
  # Sort the values so we convert to ranges and binary chop
  syscalls = sorted(syscalls, lambda x, y: cmp(x[1], y[1]))

  # Turn into a list of ranges. Keep the names for the comments
  ranges = []
  for name, value in syscalls:
    if not ranges:
      ranges.append(SyscallRange(name, value))
      continue

    last_range = ranges[-1]
    if last_range.end == value:
      last_range.add(name, value)
    else:
      ranges.append(SyscallRange(name, value))
  return ranges


# Converts the sorted ranges of allowed syscalls to a binary tree bpf
# For a single range, output a simple jump to {fail} or {allow}. We can't set
# the jump ranges yet, since we don't know the size of the filter, so use a
# placeholder
# For multiple ranges, split into two, convert the two halves and output a jump
# to the correct half
def convert_to_intermediate_bpf(ranges):
  if len(ranges) == 1:
    # We will replace {fail} and {allow} with appropriate range jumps later
    return [BPF_JGE.format(ranges[0].end, "{fail}", "{allow}") +
            ", //" + "|".join(ranges[0].names)]
  else:
    half = (len(ranges) + 1) / 2
    first = convert_to_intermediate_bpf(ranges[:half])
    second = convert_to_intermediate_bpf(ranges[half:])
    jump = [BPF_JGE.format(ranges[half].begin, len(first), 0) + ","]
    return jump + first + second


def get_blocked_syscalls(syscalls, ranges):
  # syscalls is name => number, invert the map.
  syscall_nr_map = {v: k for k, v in syscalls.iteritems()}
  last_range = SyscallRange("placeholder", -1)
  result = []
  for current_range in ranges:
    blocked_start = last_range.end
    blocked_end = current_range.begin

    for i in xrange(blocked_start, blocked_end):
      if i in syscall_nr_map:
        result.append(syscall_nr_map[i])

    last_range = current_range

  return result


def convert_ranges_to_bpf(ranges):
  bpf = convert_to_intermediate_bpf(ranges)

  # Now we know the size of the tree, we can substitute the {fail} and {allow}
  # placeholders
  for i, statement in enumerate(bpf):
    # Replace placeholder with
    # "distance to jump to fail, distance to jump to allow"
    # We will add a kill statement and an allow statement after the tree
    # With bpfs jmp 0 means the next statement, so the distance to the end is
    # len(bpf) - i - 1, which is where we will put the kill statement, and
    # then the statement after that is the allow statement
    if "{fail}" in statement and "{allow}" in statement:
      bpf[i] = statement.format(fail=str(len(bpf) - i),
                                allow=str(len(bpf) - i - 1))


  # Add the allow calls at the end. If the syscall is not matched, we will
  # continue. This allows the user to choose to match further syscalls, and
  # also to choose the action when we want to block
  bpf.append(BPF_ALLOW + ",")

  # Add check that we aren't off the bottom of the syscalls
  bpf.insert(0, BPF_JGE.format(ranges[0].begin, 0, str(len(bpf))) + ',')
  return bpf


def convert_bpf_to_output(bpf, architecture):
  header = textwrap.dedent("""\
    // Autogenerated file - edit at your peril!!

    #include <linux/filter.h>
    #include <errno.h>

    #include "seccomp_bpfs.h"
    const sock_filter {architecture}_filter[] = {{
    """).format(architecture=architecture)

  footer = textwrap.dedent("""\

    }};

    const size_t {architecture}_filter_size = sizeof({architecture}_filter) / sizeof(struct sock_filter);
    """).format(architecture=architecture)
  return header + "\n".join(bpf) + footer


def get_allowed_syscall_ranges(syscalls, syscall_files, architecture):
  allowed_names = get_allowed_names(syscall_files, architecture)
  allowed_syscalls = convert_names_to_NRs(syscalls, allowed_names)
  return convert_NRs_to_ranges(allowed_syscalls)


def construct_bpf(syscalls, architecture, ranges):
  bpf = convert_ranges_to_bpf(ranges)
  return convert_bpf_to_output(bpf, architecture)


ANDROID_SYSCALL_FILES = ["SYSCALLS.TXT",
                         "SECCOMP_WHITELIST.TXT",
                         "SECCOMP_BLACKLIST.TXT"]


POLICY_CONFIGS = [("arm", "kernel/uapi/asm-arm", []),
                  ("arm64", "kernel/uapi/asm-arm64", []),
                  ("x86", "kernel/uapi/asm-x86", ["-D__i386__"]),
                  ("x86_64", "kernel/uapi/asm-x86", []),
                  ("mips", "kernel/uapi/asm-mips", ["-D_MIPS_SIM=_MIPS_SIM_ABI32"]),
                  ("mips64", "kernel/uapi/asm-mips", ["-D_MIPS_SIM=_MIPS_SIM_ABI64"])]


def set_dir():
  # Set working directory for predictable results
  os.chdir(os.path.join(os.environ["ANDROID_BUILD_TOP"], "bionic/libc"))


def main():
  set_dir()
  for arch, header_path, switches in POLICY_CONFIGS:
    files = [open(filename) for filename in ANDROID_SYSCALL_FILES]

    syscalls = get_syscalls(header_path, switches)
    ranges = get_allowed_syscall_ranges(syscalls, files, arch)

    if sys.argv[-1] == "-v":
      blocked = get_blocked_syscalls(syscalls, ranges)
      print("Blocked syscalls on {}:".format(arch))
      for blocked_syscall in blocked:
        print("    {}".format(blocked_syscall))

    output = construct_bpf(syscalls, arch, ranges)

    # And output policy
    existing = ""
    output_path = "seccomp/{}_policy.cpp".format(arch)
    if os.path.isfile(output_path):
      existing = open(output_path).read()
    if output == existing:
      print "File " + output_path + " not changed."
    else:
      with open(output_path, "w") as output_file:
        output_file.write(output)
      print "Generated file " + output_path

if __name__ == "__main__":
  main()
