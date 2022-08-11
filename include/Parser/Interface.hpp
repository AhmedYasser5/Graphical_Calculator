#pragma once

#include "include/Union_Container.hpp"
#include <unordered_set>
#include <vector>

namespace Calculator {

template <typename T> class ParserInterface {
public:
  virtual static std::vector<NumberOperator>
  parse(const std::string &equation,
        const std::unordered_set<char> &variables) = 0;
};

} // namespace Calculator
