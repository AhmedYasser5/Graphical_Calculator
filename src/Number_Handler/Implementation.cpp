#include "Number_Handler/Implementation.hpp"
#include <cctype>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace Calculator;
using std::abs;
using std::isdigit;
using std::isinf;
using std::isnan;
using std::numeric_limits;
using std::pow;
using std::round;
using std::runtime_error;
using std::size_t;
using std::string;

#define EPSILON numeric_limits<NumberType>::epsilon()

bool NumberHandler::isNAN() const { return false; }

template <typename... Args>
bool NumberHandler::isNAN(const NumberType &number,
                          const Args &...restOfNumbers) const {
  return isnan(number) || isNAN(restOfNumbers...);
}

bool NumberHandler::isEqual(const NumberType &first,
                            const NumberType &second) const {
  NumberType error = abs(first - second);
  return error <= EPSILON || error <= abs(first) * EPSILON ||
         error <= abs(second) * EPSILON;
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
  writer << std::fixed << std::setprecision(numeric_limits<double>::digits10)
         << num;
  return writer.str();
}

NumberHandler::NumberType NumberHandler::add(const NumberType &augend,
                                             const NumberType &addend) const {
  if (isNAN(augend, addend))
    throw runtime_error("NAN error: add(" + toString(augend) + ", " +
                        toString(addend) + ")");
  return augend + addend;
}

NumberHandler::NumberType
NumberHandler::subtract(const NumberType &minuend,
                        const NumberType &subtrahend) const {
  if (isNAN(minuend, subtrahend))
    throw runtime_error("NAN error: subtract(" + toString(minuend) + ", " +
                        toString(subtrahend) + ")");
  return minuend - subtrahend;
}

NumberHandler::NumberType
NumberHandler::multiply(const NumberType &multiplicand,
                        const NumberType &multiplier) const {
  if (isNAN(multiplicand, multiplier))
    throw runtime_error("NAN error: multiply(" + toString(multiplicand) + ", " +
                        toString(multiplier) + ")");
  return multiplicand * multiplier;
}

NumberHandler::NumberType
NumberHandler::divide(const NumberType &dividend,
                      const NumberType &divisor) const {
  if (isNAN(dividend, divisor))
    throw runtime_error("NAN error: divide(" + toString(dividend) + ", " +
                        toString(divisor) + ")");
  NumberType result = dividend / divisor;
  if (isinf(result))
    throw runtime_error("Divide by zero error: divide(" + toString(dividend) +
                        ", " + toString(divisor) + ")");
  return result;
}

NumberHandler::NumberType
NumberHandler::power(const NumberType &base, const NumberType &exponent) const {
  if (isNAN(base, exponent))
    throw runtime_error("NAN error: power(" + toString(base) + ", " +
                        toString(exponent) + ")");
  NumberType power = pow(base, exponent);
  if (isNAN(power) && isEqual(round(exponent), exponent))
    power = pow(base, round(exponent));
  if (isNAN(power))
    throw runtime_error("Fractional power of negative numbers error: power(" +
                        toString(base) + ", " + toString(exponent) + ")");
  return power;
}

NumberHandler::NumberType
NumberHandler::modulo(const NumberType &dividend,
                      const NumberType &divisor) const {
  if (isNAN(dividend, divisor))
    throw runtime_error("NAN error: modulo(" + toString(dividend) + ", " +
                        toString(divisor) + ")");
  NumberType remainder = std::fmod(dividend, divisor);
  if (isNAN(remainder))
    throw runtime_error("Modulus to zero error: modulo(" + toString(dividend) +
                        ", " + toString(divisor) + ")");
  return remainder;
}

NumberHandler::NumberType
NumberHandler::negate(const NumberType &number) const {
  if (isNAN(number))
    throw runtime_error("NAN error: negate(" + toString(number) + ")");
  return -number;
}

NumberHandler::NumberType NumberHandler::log(const NumberType &power,
                                             const NumberType &base) const {
  if (isNAN(power, base))
    throw runtime_error("NAN error: log(" + toString(power) + ", " +
                        toString(base) + ")");
  NumberType logarithm = std::log(power) / std::log(base);
  if (isNAN(logarithm) && base < 0) {
    NumberType absLog = std::log(abs(power)) / std::log(abs(base));
    try {
      if (isEqual(NumberHandler::power(base, absLog), power))
        logarithm = absLog;
    } catch (...) {
    }
    if (isNAN(logarithm)) {
      try {
        if (isEqual(NumberHandler::power(base, -absLog), power))
          logarithm = -absLog;
      } catch (...) {
      }
    }
  }
  if (isNAN(logarithm) || isinf(logarithm))
    throw runtime_error("Log error: log(" + toString(power) + ", " +
                        toString(base) + ")");
  return logarithm;
}
