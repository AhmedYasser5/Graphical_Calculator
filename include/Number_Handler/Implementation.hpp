#pragma once

#include "Number_Handler/Interface.hpp"

namespace Calculator {

class NumberHandler : public NumberHandlerInterface<double> {
public:
  using NumberType = double;
  virtual ~NumberHandler() = default;
  virtual NumberType
  fromString(const std::string &str) const; // throws std::runtime_error
  virtual std::string toString(const NumberType &num) const;
  virtual NumberType add(const NumberType &first,
                         const NumberType &second) const;
  virtual NumberType subtract(const NumberType &first,
                              const NumberType &second) const;
  virtual NumberType multiply(const NumberType &first,
                              const NumberType &second) const;
  virtual NumberType
  divide(const NumberType &first,
         const NumberType &second) const; // throws std::runtime_error
  virtual NumberType
  power(const NumberType &first,
        const NumberType &second) const; // throws std::runtime_error
  virtual NumberType
  modulo(const NumberType &first,
         const NumberType &second) const; // throws std::runtime_error
  virtual NumberType negate(const NumberType &number) const;
  virtual NumberType
  log(const NumberType &first,
      const NumberType &second) const; // throws std::runtime_error
};

} // namespace Calculator
