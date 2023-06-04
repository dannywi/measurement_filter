#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace mf {

template <typename T>
class filter_base {
 protected:
  T current_estimate_{};

 public:
  virtual void update(T) = 0;
  virtual T current_estimate() const = 0;
};

template <typename T>
class filter_avg : public filter_base<T> {
  unsigned int count_{0};

 public:
  void update(T new_measurement) override {
    T alpha = 1.0 / static_cast<T>(++count_);
    this->current_estimate_ = alpha * new_measurement + (1.0 - alpha) * this->current_estimate_;
  }

  T current_estimate() const override { return this->current_estimate_; }
};

// todo: adapt to filter_base template
template <typename T, typename TM = T>
class filter_alpha_beta {
  // high values for high precision (but prone to fluctuations)
  // low values for low precision (but slow to adapt to changes)
  float alpha_{1};  // weight of new observation
  float beta_{1};   // weight of new observed velocity

  T current_estimate_;  // todo: rename, should include "value"
  T velocity_;          // todo: rename, this is also current estimate

  TM last_time_{0};  // todo: abstract the concept, denominator of velocity

 public:
  filter_alpha_beta(float alpha, float beta) : alpha_(alpha), beta_(beta) {
    if (alpha_ < 0 || beta_ < 0) throw std::runtime_error("invalid alpha beta");
  }

  void init(T value, T velocity) {
    current_estimate_ = value;
    velocity_ = velocity;
  }

  void update(T new_measurement, TM time) {
    if (time < last_time_) { return; }
    T estimate_from_prev = current_estimate(time);
    T estimate_error = new_measurement - estimate_from_prev;

    current_estimate_ = estimate_from_prev + alpha_ * estimate_error;
    velocity_ += beta_ * estimate_error / (time - last_time_);

    last_time_ = time;
  }

  T current_estimate(T time) const {
    // might not be necessary to check in theory, but TM can be unsigned.
    if (time < last_time_) { return current_estimate_; }
    return current_estimate_ + velocity_ * std::max(0.0, time - last_time_);
  }
};

}  // namespace mf
