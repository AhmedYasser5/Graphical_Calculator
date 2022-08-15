#pragma once

#include "Number_Handler/Interface.hpp"
#include <string>

namespace Calculator {

class NumberHandler : public NumberHandlerInterface<double> {
public:
  virtual ~NumberHandler() = default;
  virtual double
  fromString(const std::string &str) const; // throws std::runtime_error
  virtual std::string toString(const double &num) const;
  virtual double add(const double &first, const double &second) const;
  virtual double subtract(const double &first, const double &second) const;
  virtual double multiply(const double &first, const double &second) const;
  virtual double
  divide(const double &first,
         const double &second) const; // throws std::runtime_error
  virtual double power(const double &first,
                       const double &second) const; // throws std::runtime_error
  virtual double
  modulo(const double &first,
         const double &second) const; // throws std::runtime_error
};

} // namespace Calculator
