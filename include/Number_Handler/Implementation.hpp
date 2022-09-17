#pragma once

#include "Number_Handler/Interface.hpp"

namespace Calculator {

using std::string;

class NumberHandler : public NumberHandlerInterface<double> {
protected:
  using NumberType = double;

  bool isNAN() const;
  template <typename... Args>
  bool isNAN(const NumberType &number, const Args &...restOfNumbers) const;
  bool isEqual(const NumberType &first, const NumberType &second) const;

public:
  NumberType fromString(const string &str) const; // throws std::runtime_error
  string toString(const NumberType &num) const;
  NumberType add(const NumberType &augend,
                 const NumberType &addend) const; // throws std::runtime_error
  NumberType
  subtract(const NumberType &minuend,
           const NumberType &subtrahend) const; // throws std::runtime_error
  NumberType
  multiply(const NumberType &multiplicand,
           const NumberType &multiplier) const; // throws std::runtime_error
  NumberType
  divide(const NumberType &dividend,
         const NumberType &divisor) const; // throws std::runtime_error
  NumberType
  power(const NumberType &base,
        const NumberType &exponent) const; // throws std::runtime_error
  NumberType
  modulo(const NumberType &dividend,
         const NumberType &divisor) const; // throws std::runtime_error
  NumberType
  negate(const NumberType &number) const; // throws std::runtime_error
  NumberType log(const NumberType &power,
                 const NumberType &base) const; // throws std::runtime_error
};

} // namespace Calculator
