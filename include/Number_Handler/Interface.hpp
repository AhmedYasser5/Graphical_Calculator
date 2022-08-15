#pragma once

#include <string>

namespace Calculator {

template <typename N> class NumberHandlerInterface {
public:
  virtual ~NumberHandlerInterface() = default;
  virtual N fromString(const std::string &str) const = 0;
  virtual std::string toString(const N &num) const = 0;
  virtual N add(const N &first, const N &second) const = 0;
  virtual N subtract(const N &first, const N &second) const = 0;
  virtual N multiply(const N &first, const N &second) const = 0;
  virtual N divide(const N &first, const N &second) const = 0;
  virtual N power(const N &first, const N &second) const = 0;
  virtual N modulo(const N &first, const N &second) const = 0;
};

} // namespace Calculator
