#include <gtest/gtest.h>

#include <iostream>

#include "filter.hpp"

constexpr double EPS = 0.05;

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

TEST(FILTER, AlphaBeta1) {
  mf::filter_alpha_beta<double> f{0.2, 0.1};
  f.init(30000, 40);
  EXPECT_NEAR(f.current_estimate(0), 30000, EPS);
  EXPECT_NEAR(f.current_estimate(1), 30040, EPS);
  EXPECT_NEAR(f.current_estimate(5), 30200, EPS);

  f.update(30200, 5);
  EXPECT_NEAR(f.current_estimate(5), 30200, EPS);

  f.update(30300, 10);
  EXPECT_NEAR(f.current_estimate(10), 30380, EPS);
}

TEST(FILTER, AlphaBeta2) {
  mf::filter_alpha_beta<double, uint8_t> f{0.2, 0.1};

  constexpr uint8_t INTERVAL = 5;

  struct test {
    double new_val;
    double estimate_exp;
  };

  // clang-format off
  std::vector<test> tests = {
      {30171, 30194.2},
      {30353, 30383.64},
      {30756, 30612.73},
      {30799, 30818.93},
      {31018, 31025.7},
      {31278, 31242.3},
      {31276, 31418.8},
      {31379, 31566.3},
      {31748, 31739.4},
      {32175, 31964.1},
  };
  // clang-format on

  f.init(30000, 40);
  uint8_t tm = 0;
  for (auto t : tests) {
    tm += INTERVAL;
    f.update(t.new_val, tm);
    EXPECT_NEAR(f.current_estimate(tm), t.estimate_exp, EPS);
  }
}

TEST(FILTER, AlphaBeta3) {
  mf::filter_alpha_beta<double, uint8_t> f{0.2, 0.1};

  constexpr uint8_t INTERVAL = 5;

  struct test {
    double new_val;
    double estimate_exp;
  };

  // Test with accelerating object, see the slow adaptation
  // clang-format off
  std::vector<test> tests = {
      {30221, 30244.2},
      {30453, 30483.64},
      {30906, 30762.7},
      {30999, 31018.93},
      {31368, 31295.7},
      {31978, 31646.3},
      {32526, 32069.6},
      {33379, 32624.5},
      {34698, 33407.6},
      {36275, 34478.6},
  };
  // clang-format on

  f.init(30000, 50);
  uint8_t tm = 0;
  for (auto t : tests) {
    tm += INTERVAL;
    f.update(t.new_val, tm);
    EXPECT_NEAR(f.current_estimate(tm), t.estimate_exp, EPS);
  }
}
