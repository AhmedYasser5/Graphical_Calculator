#pragma once

#include "Evaluator/Interface.hpp"
#include <stack>

namespace Calculator {

class Evaluator : public EvaluatorInterface<double, std::string, std::string> {
private:
  using NumberType = double;
  using VariableType = std::string;
  using FunctionType = std::string;
  using EquationElement =
      EquationElement<NumberType, VariableType, FunctionType>;

  std::unique_ptr<NumberHandlerInterface<NumberType>> operationsHandler;
  std::stack<NumberType> stackedNumbers;

  void processFunctions(const FunctionType &func);
  void processVariables(
      const VariableType &var,
      const std::unordered_map<std::string, NumberType> &variables);

public:
  Evaluator(
      std::unique_ptr<NumberHandlerInterface<NumberType>> operationsHandler);
  virtual ~Evaluator() = default;
  virtual NumberType
  evaluate(const std::vector<EquationElement> &equation,
           const std::unordered_map<std::string, NumberType> &variables =
               {}); // throws different exceptions
};

} // namespace Calculator
