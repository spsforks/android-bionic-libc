#include <stdio.h>
#include <math.h>

#include <gtest/gtest.h>

TEST(libm, cos_override) {
  ASSERT_TRUE(isnan(cos(1.0)));
}

TEST(libm, cosf_override) {
  ASSERT_TRUE(isnan(cosf(1.0)));
}

TEST(libm, cosl_override) {
  ASSERT_TRUE(isnan(cosl(1.0)));
}
