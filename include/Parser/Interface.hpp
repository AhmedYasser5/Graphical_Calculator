#pragma once

#include "Number_Handler/Interface.hpp"
#include "Union_Container.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace Calculator {

template <typename N, typename O = std::string, typename F = std::string>
class ParserInterface {
protected:
  typename std::unique_ptr<NumberHandlerInterface<N>> reader;
  typename std::vector<UnionContainer<N, O, F>> parsedEquation;

public:
  virtual ~ParserInterface() = default;
  virtual void parse(const std::string &equation,
                     const std::unordered_set<std::string> &variables = {}) = 0;
  virtual const typename std::vector<UnionContainer<N, O, F>> &
  getParsedEquation() const = 0;
};

} // namespace Calculator
