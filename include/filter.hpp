#pragma once

namespace mf {
template <typename T>
class filter_avg {
  T latest_{0};
  unsigned int count_{0};

 public:
  void update(T new_measurement) {
    T alpha = 1.0 / static_cast<T>(++count_);
    latest_ = alpha * new_measurement + (1.0 - alpha) * latest_;
  }

  T current_estimate() const { return latest_; }
};
}  // namespace mf
