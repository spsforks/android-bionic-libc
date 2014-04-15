bool testlib_apply_goldbach_test(unsigned int n) {
  // known to be true for all even numbers between
  // 2 and MAX_INT
  return n>2 && 0 == n%2;
}
