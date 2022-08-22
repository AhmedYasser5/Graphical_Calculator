#pragma once

#include <string>

namespace Calculator {

template <typename N> class NumberHandlerInterface {
public:
  virtual ~NumberHandlerInterface() = default;
  virtual N fromString(const std::string &str) const = 0;
  virtual std::string toString(const N &num) const = 0;
  virtual N add(const N &augend, const N &addend) const = 0;
  virtual N subtract(const N &minuend, const N &subtrahend) const = 0;
  virtual N multiply(const N &multiplicand, const N &multiplier) const = 0;
  virtual N divide(const N &dividend, const N &divisor) const = 0;
  virtual N power(const N &base, const N &exponent) const = 0;
  virtual N modulo(const N &dividend, const N &divisor) const = 0;
  virtual N negate(const N &number) const = 0;
  virtual N log(const N &power, const N &base) const = 0;
};

} // namespace Calculator
