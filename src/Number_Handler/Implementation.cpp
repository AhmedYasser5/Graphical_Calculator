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

double NumberHandler::fromString(const string &str) const {
  double num = 0;
  size_t id = 0;
  while (id < str.size() && isdigit(str[id]))
    num = num * 10 + str[id++] - '0';
  if (id < str.size()) {
    if (str[id++] != '.')
      throw runtime_error("Not a valid decimal place: " + str);
    double base = .1;
    while (id < str.size() && isdigit(str[id])) {
      num += (str[id++] - '0') * base;
      base /= 10;
    }
    if (id < str.size())
      throw runtime_error("Not a valid number: " + str);
  }
  return num;
}

string NumberHandler::toString(const double &num) const {
  std::stringstream writer;
  writer << num;
  return writer.str();
}

double NumberHandler::add(const double &first, const double &second) const {
  return first + second;
}

double NumberHandler::subtract(const double &first,
                               const double &second) const {
  return first - second;
}

double NumberHandler::multiply(const double &first,
                               const double &second) const {
  return first * second;
}

double NumberHandler::divide(const double &first, const double &second) const {
  double result = first / second;
  if (std::isinf(result))
    throw runtime_error("Divide by zero error: divide(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}

double NumberHandler::power(const double &first, const double &second) const {
  double result = std::pow(first, second);
  if (isnan(result))
    throw runtime_error("Fractional power to negative numbers error: power(" +
                        toString(first) + ", " + toString(second) + ")");
  return result;
}

double NumberHandler::modulo(const double &first, const double &second) const {
  double result = std::fmod(first, second);
  if (isnan(result))
    throw runtime_error("Modulus to zero error: modulo(" + toString(first) +
                        ", " + toString(second) + ")");
  return result;
}
