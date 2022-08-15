#pragma once

#include "Number_Handler/Interface.hpp"
#include "Union_Container.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Calculator {

template <typename N, typename O = std::string, typename F = std::string>
class EvaluatorInterface {
protected:
  typename std::unique_ptr<NumberHandlerInterface<N>> operationsHandler;

public:
  virtual ~EvaluatorInterface() = default;
  virtual N evaluate(
      const typename std::vector<UnionContainer<N, O, F>> &equation,
      const typename std::unordered_map<std::string, N> &variables = {}) = 0;
};

} // namespace Calculator
