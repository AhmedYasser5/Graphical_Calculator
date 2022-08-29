#include "Evaluator/Implementation.hpp"
#include <cstddef>
#include <stdexcept>

using namespace Calculator;
using std::runtime_error;

Evaluator::Evaluator(
    unique_ptr<NumberHandlerInterface<NumberType>> operationsHandler)
    : operationsHandler(std::move(operationsHandler)) {}

bool Evaluator::processOneInputFunctions(const FunctionType &func,
                                         const NumberType &number) {
  NumberType answer;
  if (func == "neg")
    answer = operationsHandler->negate(number);
  else
    return false;
  stackedNumbers.emplace(answer);
  return true;
}

bool Evaluator::processTwoInputFunctions(const FunctionType &func,
                                         const NumberType &first,
                                         const NumberType &second) {
  NumberType answer;
  if (func == "+")
    answer = operationsHandler->add(first, second);
  else if (func == "-")
    answer = operationsHandler->subtract(first, second);
  else if (func == "*")
    answer = operationsHandler->multiply(first, second);
  else if (func == "/")
    answer = operationsHandler->divide(first, second);
  else if (func == "%")
    answer = operationsHandler->modulo(first, second);
  else if (func == "^")
    answer = operationsHandler->power(first, second);
  else if (func == "log")
    answer = operationsHandler->log(first, second);
  else
    return false;
  stackedNumbers.emplace(answer);
  return true;
}

void Evaluator::processFunctions(const FunctionType &func) {
  if (stackedNumbers.empty())
    throw runtime_error("No numbers were found for the function " + func);
  NumberType number = stackedNumbers.top();
  stackedNumbers.pop();
  bool isProcessed = processOneInputFunctions(func, number);
  if (isProcessed)
    return;
  if (stackedNumbers.empty())
    throw runtime_error("Only one number was found for the function " + func);
  auto &second = number;
  NumberType first = stackedNumbers.top();
  stackedNumbers.pop();
  isProcessed = processTwoInputFunctions(func, first, second);
  if (isProcessed)
    return;
  throw runtime_error("Unknown function: " + func);
}

void Evaluator::processVariables(
    const VariableType &var,
    const unordered_map<string, NumberType> &variables) {
  auto it = variables.find(var);
  if (it != variables.end())
    stackedNumbers.push(it->second);
  else
    throw runtime_error("No variable " + var + " is defined");
}

Evaluator::NumberType
Evaluator::evaluate(const std::vector<EquationElement> &equation,
                    const unordered_map<string, NumberType> &variables) {
  while (!stackedNumbers.empty())
    stackedNumbers.pop();
  for (size_t i = 0; i < equation.size(); i++) {
    if (equation[i].getState() == EquationElement::NUMBER)
      stackedNumbers.push(equation[i].getNumber());
    else if (equation[i].getState() == EquationElement::FUNCTION)
      processFunctions(equation[i].getFunction());
    else if (equation[i].getState() == EquationElement::VARIABLE)
      processVariables(equation[i].getVariable(), variables);
    else
      throw runtime_error("Unknown part of the equation");
  }
  if (stackedNumbers.empty())
    throw runtime_error("Unknown error (No answer was found at the end)");
  if (stackedNumbers.size() > 1)
    throw runtime_error("Found more than one answer at the end");
  return stackedNumbers.top();
}
