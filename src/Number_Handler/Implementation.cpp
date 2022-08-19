#include "Number_Handler/Implementation.hpp"
#include <cctype>
#include <cmath>
#include <cstddef>
#include <sstream>
#include <stdexcept>

using namespace Calculator;
using std::isdigit;
using std::isnan;
using std::runtime_error;
using std::size_t;
using std::string;

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
  writer << num;
  return writer.str();
}

NumberHandler::NumberType NumberHandler::add(const NumberType &first,
                                             const NumberType &second) const {
  return first + second;
}

NumberHandler::NumberType
NumberHandler::subtract(const NumberType &first,
                        const NumberType &second) const {
  return first - second;
}

NumberHandler::NumberType
NumberHandler::multiply(const NumberType &first,
                        const NumberType &second) const {
  return first * second;
}

NumberHandler::NumberType
NumberHandler::divide(const NumberType &first, const NumberType &second) const {
  NumberType result = first / second;
  if (std::isinf(result))
    throw runtime_error("Divide by zero error: divide(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}

NumberHandler::NumberType NumberHandler::power(const NumberType &first,
                                               const NumberType &second) const {
  NumberType result = std::pow(first, second);
  if (isnan(result))
    throw runtime_error("Fractional power to negative numbers error: power(" +
                        toString(first) + ", " + toString(second) + ")");
  return result;
}

NumberHandler::NumberType
NumberHandler::modulo(const NumberType &first, const NumberType &second) const {
  NumberType result = std::fmod(first, second);
  if (isnan(result))
    throw runtime_error("Modulus to zero error: modulo(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}

NumberHandler::NumberType
NumberHandler::negate(const NumberType &number) const {
  return -number;
}
