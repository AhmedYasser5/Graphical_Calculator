#pragma once

#include <string>

namespace Calculator {

template <typename T> class NumberHandlerInterface {
public:
  virtual static T fromString(const std::string &str) = 0;
  virtual static std::string toString(const T &num) = 0;
  virtual static T add(const T &first, const T &second) = 0;
  virtual static T subtract(const T &first, const T &second) = 0;
  virtual static T multiply(const T &first, const T &second) = 0;
  virtual static T divide(const T &first, const T &second) = 0;
  virtual static T power(const T &first, const T &second) = 0;
  virtual static T modulo(const T &first, const T &second) = 0;
};

} // namespace Calculator
