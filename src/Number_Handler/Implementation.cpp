#include "Number_Handler/Implementation.hpp"
#include <cctype>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace Calculator;
using std::isdigit;
using std::isnan;
using std::pow;
using std::runtime_error;
using std::size_t;
using std::string;

bool NumberHandler::isNAN() const { return false; }

template <typename... Args>
bool NumberHandler::isNAN(const NumberType &number,
                          const Args &...restOfNumbers) const {
  return isnan(number) || isNAN(restOfNumbers...);
}

NumberHandler::NumberType NumberHandler::fromString(const string &str) const {
  NumberType num = 0;
  size_t id = 0;
  while (id < str.size() && isdigit(str[id]))
    num = num * 10 + str[id++] - '0';
  if (id < str.size()) {
    if (str[id++] != '.')
      throw runtime_error("Not a valid decimal place: " + str);
    NumberType base = .1;
    while (id < str.size() && isdigit(str[id])) {
      num += (str[id++] - '0') * base;
      base /= 10;
    }
    if (id < str.size())
      throw runtime_error("Not a valid number: " + str);
  }
  return num;
}

string NumberHandler::toString(const NumberType &num) const {
  std::stringstream writer;
  writer << std::fixed
         << std::setprecision(std::numeric_limits<double>::digits10) << num;
  return writer.str();
}

NumberHandler::NumberType NumberHandler::add(const NumberType &first,
                                             const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: add(" + toString(first) + ", " +
                        toString(second) + ")");
  return first + second;
}

NumberHandler::NumberType
NumberHandler::subtract(const NumberType &first,
                        const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: subtract(" + toString(first) + ", " +
                        toString(second) + ")");
  return first - second;
}

NumberHandler::NumberType
NumberHandler::multiply(const NumberType &first,
                        const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: multiply(" + toString(first) + ", " +
                        toString(second) + ")");
  return first * second;
}

NumberHandler::NumberType
NumberHandler::divide(const NumberType &first, const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: divide(" + toString(first) + ", " +
                        toString(second) + ")");
  NumberType result = first / second;
  if (std::isinf(result))
    throw runtime_error("Divide by zero error: divide(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}

NumberHandler::NumberType NumberHandler::power(const NumberType &first,
                                               const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: power(" + toString(first) + ", " +
                        toString(second) + ")");
  NumberType Base = first, Power = second;
  if (Power < 0) {
    Base = 1 / Base;
    Power = -Power;
  }
  NumberType Result;
  if (Base >= 0)
    Result = pow(Base, Power);
  else {
    Result = pow(-Base, Power);
    if (pow(-Result, 1 / Power) == Base)
      Result = -Result;
    else if (Result != pow(Result, 1 / Power))
      Result = pow(Base, Power);
  }
  if (isNAN(Result))
    throw runtime_error("Fractional power of negative numbers error: power(" +
                        toString(first) + ", " + toString(second) + ")");
  return Result;
}

NumberHandler::NumberType
NumberHandler::modulo(const NumberType &first, const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: modulo(" + toString(first) + ", " +
                        toString(second) + ")");
  NumberType result = std::fmod(first, second);
  if (isnan(result))
    throw runtime_error("Modulus to zero error: modulo(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}

NumberHandler::NumberType
NumberHandler::negate(const NumberType &number) const {
  if (isNAN(number))
    throw runtime_error("NAN error: negate(" + toString(number) + ")");
  return -number;
}

NumberHandler::NumberType NumberHandler::log(const NumberType &first,
                                             const NumberType &second) const {
  if (isNAN(first, second))
    throw runtime_error("NAN error: log(" + toString(first) + ", " +
                        toString(second) + ")");
  NumberType result = std::log(first) / std::log(second);
  if (isnan(result))
    throw runtime_error("Log error: log(" + toString(first) + ", " +
                        toString(second) + ")");
  return result;
}
