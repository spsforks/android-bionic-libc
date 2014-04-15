extern bool testlib_goldbach_test(unsigned int n);

unsigned int testlib_collatz_next(unsigned int n) {
  bool f = testlib_goldbach_test(n);
  return (f || n%2 != 0) ? n*3+1 : n/2;
}
