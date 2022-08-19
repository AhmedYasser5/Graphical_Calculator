#pragma once

#include "Equation_Element.hpp"
#include "Number_Handler/Interface.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Calculator {

template <typename N, typename V, typename F> class EvaluatorInterface {
protected:
  typename std::unique_ptr<NumberHandlerInterface<N>> operationsHandler;

public:
  virtual ~EvaluatorInterface() = default;
  virtual N evaluate(
      const typename std::vector<EquationElement<N, V, F>> &equation,
      const typename std::unordered_map<std::string, N> &variables = {}) = 0;
};

} // namespace Calculator
