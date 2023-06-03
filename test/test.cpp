#include <gtest/gtest.h>

#include <iostream>

#include "filter.hpp"

constexpr double EPS = 1e-2;

TEST(FILTER, Avg) {
  mf::filter_avg<double> f;
  f.update(33);
  EXPECT_NEAR(f.current_estimate(), 33, EPS);
  f.update(33);
  EXPECT_NEAR(f.current_estimate(), 33, EPS);
  f.update(30);
  EXPECT_NEAR(f.current_estimate(), 32, EPS);
}

TEST(FILTER, Avg2) {
  mf::filter_avg<double> f;
  struct test {
    double new_val;
    double estimate_exp;
  };

  // clang-format off
  std::vector<test> tests {
    {996, 996},
    {994, 995},
    {1021, 1003.67},
    {1000, 1002.75},
    {1002, 1002.6},
    {1010, 1003.83},
    {983, 1000.86},
    {971, 997.125},
    {993, 996.67},
    {1023, 999.3},
  };
  // clang-format on

  for (auto t : tests) {
    f.update(t.new_val);
    EXPECT_NEAR(f.current_estimate(), t.estimate_exp, EPS);
  }
}
