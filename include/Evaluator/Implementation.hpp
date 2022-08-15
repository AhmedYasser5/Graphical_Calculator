#pragma once

#include "Evaluator/Interface.hpp"
#include <stack>

namespace Calculator {

class Evaluator : public EvaluatorInterface<double, std::string, std::string> {
private:
  using UnionContainer = UnionContainer<double, std::string, std::string>;

  std::unique_ptr<NumberHandlerInterface<double>> operationsHandler;
  std::stack<double> stackedNumbers;

  void processOperator(const std::string &op);
  void
  processFunction(const std::string &func,
                  const std::unordered_map<std::string, double> &variables);

public:
  Evaluator(std::unique_ptr<NumberHandlerInterface<double>> operationsHandler);
  virtual ~Evaluator() = default;
  virtual double evaluate(const std::vector<UnionContainer> &equation,
                          const std::unordered_map<std::string, double>
                              &variables = {}); // throws different exceptions
};

} // namespace Calculator
