#pragma once

#include "Number_Handler/Interface.hpp"

namespace Calculator {

class NumberHandler : public NumberHandlerInterface<double> {
protected:
  using NumberType = double;

  bool isNAN() const;
  template <typename... Args>
  bool isNAN(const NumberType &number, const Args &...restOfNumbers) const;
  bool isEqual(const NumberType &first, const NumberType &second) const;

public:
  virtual ~NumberHandler() = default;
  virtual NumberType
  fromString(const std::string &str) const; // throws std::runtime_error
  virtual std::string toString(const NumberType &num) const;
  virtual NumberType
  add(const NumberType &augend,
      const NumberType &addend) const; // throws std::runtime_error
  virtual NumberType
  subtract(const NumberType &minuend,
           const NumberType &subtrahend) const; // throws std::runtime_error
  virtual NumberType
  multiply(const NumberType &multiplicand,
           const NumberType &multiplier) const; // throws std::runtime_error
  virtual NumberType
  divide(const NumberType &dividend,
         const NumberType &divisor) const; // throws std::runtime_error
protected:
  NumberType integerPower(const NumberType &base,
                          const NumberType &exponent) const;
  NumberType fractionPower(const NumberType &base,
                           const NumberType &exponent) const;

public:
  virtual NumberType
  power(const NumberType &base,
        const NumberType &exponent) const; // throws std::runtime_error
  virtual NumberType
  modulo(const NumberType &dividend,
         const NumberType &divisor) const; // throws std::runtime_error
  virtual NumberType
  negate(const NumberType &number) const; // throws std::runtime_error
  virtual NumberType
  log(const NumberType &power,
      const NumberType &base) const; // throws std::runtime_error
};

} // namespace Calculator
