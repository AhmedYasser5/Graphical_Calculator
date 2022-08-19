#pragma once

#include "Equation_Element.hpp"
#include "Number_Handler/Interface.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace Calculator {

template <typename N, typename V, typename F> class ParserInterface {
protected:
  typename std::unique_ptr<NumberHandlerInterface<N>> reader;
  typename std::vector<EquationElement<N, V, F>> parsedEquation;

public:
  virtual ~ParserInterface() = default;
  virtual void parse(const std::string &equation,
                     const std::unordered_set<std::string> &variables = {}) = 0;
  virtual const typename std::vector<EquationElement<N, V, F>> &
  getParsedEquation() const = 0;
};

} // namespace Calculator
