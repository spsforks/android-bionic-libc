#!/usr/bin/env python
# Unit tests for genseccomp.py

import unittest
import genseccomp

class TestGenseccomp(unittest.TestCase):
  def setUp(self):
    genseccomp.setDir()

  def test_get_names(self):
    names = genseccomp.get_names(genseccomp.android_syscall_files,
                                 genseccomp.arm_architecture)

    names64 = genseccomp.get_names(genseccomp.android_syscall_files,
                                   genseccomp.arm64_architecture)

    self.assertIn("fchown", names64)
    self.assertNotIn("fchown", names)
    self.assertIn("_llseek", names)
    self.assertNotIn("_llseek", names64)
    self.assertIn("read", names)
    self.assertIn("read", names64)

  def test_convert_names_to_NRs(self):
    self.assertEquals(genseccomp.convert_names_to_NRs(["open"],
                                                      genseccomp.arm_headers),
                      [("open", 5)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["__ARM_NR_set_tls"],
                                                      genseccomp.arm_headers),
                      [('__ARM_NR_set_tls', 983045)])

    self.assertEquals(genseccomp.convert_names_to_NRs(["openat"],
                                                      genseccomp.arm64_headers),
                      [("openat", 56)])

  def test_convert_NRs_to_ranges(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    self.assertEquals(len(ranges), 1)
    self.assertEquals(ranges[0].begin, 1)
    self.assertEquals(ranges[0].end, 3)
    self.assertItemsEqual(ranges[0].names, ["a", "b"])

    ranges = genseccomp.convert_NRs_to_ranges([("b", 3), ("a", 1)])
    self.assertEquals(len(ranges), 2)
    self.assertEquals(ranges[0].begin, 1)
    self.assertEquals(ranges[0].end, 2)
    self.assertItemsEqual(ranges[0].names, ["a"])

  def test_convert_to_bpf(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    bpf = genseccomp.convert_to_intermediate_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, {fail}, {allow}), //a|b'])

    ranges = genseccomp.convert_NRs_to_ranges([("b", 3), ("a", 1)])
    bpf = genseccomp.convert_to_intermediate_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 2, {fail}, {allow}), //a',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 4, {fail}, {allow}), //b'])

  def test_convert_range_to_bpf(self):
    ranges = genseccomp.convert_NRs_to_ranges([("b", 2), ("a", 1)])
    bpf = genseccomp.convert_ranges_to_bpf(ranges)
    self.assertEquals(bpf, ['BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 1, 0, 1),',
                            'BPF_JUMP(BPF_JMP|BPF_JGE|BPF_K, 3, 1, 0), //a|b',
                            'BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW),'])


if __name__ == '__main__':
  unittest.main()
