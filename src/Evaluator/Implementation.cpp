#include "Evaluator/Implementation.hpp"
#include <cstddef>
#include <stdexcept>

using namespace Calculator;
using std::runtime_error;
using std::size_t;
using std::string;

Evaluator::Evaluator(
    std::unique_ptr<NumberHandlerInterface<double>> operationsHandler)
    : operationsHandler(std::move(operationsHandler)) {}

void Evaluator::processOperator(const string &op) {
  if (stackedNumbers.empty())
    throw runtime_error("No numbers found for the operator " + op);
  double second = stackedNumbers.top();
  stackedNumbers.pop();
  if (stackedNumbers.empty())
    throw runtime_error("Only one number found for the operator " + op);
  double first = stackedNumbers.top();
  stackedNumbers.pop();
  double answer;
  if (op == "+")
    answer = operationsHandler->add(first, second);
  else if (op == "-")
    answer = operationsHandler->subtract(first, second);
  else if (op == "*")
    answer = operationsHandler->multiply(first, second);
  else if (op == "/")
    answer = operationsHandler->divide(first, second);
  else if (op == "%")
    answer = operationsHandler->modulo(first, second);
  else if (op == "^")
    answer = operationsHandler->power(first, second);
  else
    throw runtime_error("Unknown operator " + op);
  stackedNumbers.push(answer);
}

void Evaluator::processFunction(
    const string &func, const std::unordered_map<string, double> &variables) {
  auto it = variables.find(func);
  if (it != variables.end()) {
    stackedNumbers.push(it->second);
    return;
  }
  if (stackedNumbers.empty())
    throw runtime_error("No number provided for the function " + func);
  double num = stackedNumbers.top();
  stackedNumbers.pop();
  if (func == "+")
    ;
  else if (func == "-")
    num *= -1;
  else
    throw runtime_error("Unknown function " + func);
  stackedNumbers.push(num);
}

double
Evaluator::evaluate(const std::vector<UnionContainer> &equation,
                    const std::unordered_map<string, double> &variables) {
  for (size_t i = 0; i < equation.size(); i++) {
    if (equation[i].getState() == UnionContainer::NUMBER)
      stackedNumbers.push(equation[i].getNumber());
    else if (equation[i].getState() == UnionContainer::OPERATOR)
      processOperator(equation[i].getOperator());
    else if (equation[i].getState() == UnionContainer::FUNCTION)
      processFunction(equation[i].getFunction(), variables);
    else
      throw runtime_error("Unknown part of the equation");
  }
  if (stackedNumbers.empty())
    throw runtime_error("Unknown error (No answer found at the end)");
  if (stackedNumbers.size() > 1)
    throw runtime_error("Found more than one answer at the end");
  return stackedNumbers.top();
}
