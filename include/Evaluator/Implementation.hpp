#pragma once

#include "Evaluator/Interface.hpp"
#include <cstddef>
#include <stack>

namespace Calculator {

using std::size_t;
using std::stack;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

class Evaluator : public EvaluatorInterface<double, string, string> {
protected:
  using NumberType = double;
  using VariableType = string;
  using FunctionType = string;
  using EquationElement =
      EquationElement<NumberType, VariableType, FunctionType>;

  unique_ptr<NumberHandlerInterface<NumberType>> operationsHandler;
  stack<NumberType> stackedNumbers;

  bool processOneInputFunctions(const FunctionType &func,
                                const NumberType &number);
  bool processTwoInputFunctions(const FunctionType &func,
                                const NumberType &first,
                                const NumberType &second);
  void processFunctions(const FunctionType &func);
  void processVariables(const VariableType &var,
                        const unordered_map<string, NumberType> &variables);

public:
  Evaluator(unique_ptr<NumberHandlerInterface<NumberType>> operationsHandler);
  virtual ~Evaluator() = default;
  virtual NumberType
  evaluate(const vector<EquationElement> &equation,
           const unordered_map<string, NumberType> &variables =
               {}); // throws different exceptions
};

} // namespace Calculator
