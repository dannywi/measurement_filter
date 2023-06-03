#pragma once

namespace mf {

template <typename T>
class filter_base {
 protected:
  T latest_{};

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
    this->latest_ = alpha * new_measurement + (1.0 - alpha) * this->latest_;
  }

  T current_estimate() const override { return this->latest_; }
};
}  // namespace mf
