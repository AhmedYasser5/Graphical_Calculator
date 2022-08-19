#include "Parser/Implementation.hpp"
#include <cctype>
#include <stdexcept>

using namespace Calculator;
using std::isalnum;
using std::isalpha;
using std::isdigit;
using std::runtime_error;
using std::size_t;
using std::stack;
using std::string;
using std::unordered_set;
using std::vector;

Parser::Parser(
    std::unique_ptr<NumberHandlerInterface<NumberType>> numberHandler)
    : reader(std::move(numberHandler)) {
  operatorDependecies["+"] = {"+", "-", "*", "/", "%", "^"};
  operatorDependecies["-"] = {"+", "-", "*", "/", "%", "^"};
  operatorDependecies["*"] = {"*", "/", "%", "^"};
  operatorDependecies["/"] = {"*", "/", "%", "^"};
  operatorDependecies["%"] = {"*", "/", "%", "^"};
  operatorDependecies["^"] = {};
  functions.insert("neg");
}

void Parser::readWhitespaces(const string &equation, size_t &index) const {
  while (index < equation.size() && std::isblank(equation[index]))
    ++index;
}

void Parser::readOperators(const string &equation, size_t &index) {
  string op;
  while (index < equation.size() &&
         operatorDependecies.find(op) == operatorDependecies.end() &&
         !isalnum(equation[index]))
    op += equation[index++];
  auto it = operatorDependecies.find(op);
  if (it == operatorDependecies.end())
    throw runtime_error("Unknown operator: " + op);
  auto &dependencies = it->second;
  while (!stackedFunctions.empty() &&
         dependencies.find(stackedFunctions.top()) != dependencies.end()) {
    parsedEquation.emplace_back();
    parsedEquation.back().updateFunction(stackedFunctions.top());
    stackedFunctions.pop();
  }
  stackedFunctions.emplace(op);
}

void Parser::readSigns(const string &equation, size_t &index) {
  char sign = +1;
  while (index < equation.size()) {
    readWhitespaces(equation, index);
    if (index == equation.size())
      break;
    if (equation[index] == '-')
      sign *= -1;
    else if (equation[index] != '+')
      break;
    ++index;
  }
  if (sign == -1)
    stackedFunctions.emplace("neg");
}

void Parser::readNumbers(const string &equation, size_t &index) {
  string num;
  while (index < equation.size() &&
         (isdigit(equation[index]) || equation[index] == '.'))
    num += equation[index++];
  parsedEquation.emplace_back();
  parsedEquation.back().updateNumber(reader->fromString(num));
}

void Parser::readVariablesAndFunctions(const string &equation, size_t &index,
                                       const unordered_set<string> &variables) {
  parsedEquation.emplace_back();
  string temp;
  while (index < equation.size() && isalpha(equation[index]))
    temp += equation[index++];
  if (functions.find(temp) != functions.end()) {
    parsedEquation.back().updateFunction(temp);
    return;
  }
  auto &var = temp;
  auto it = variables.find(var);
  if (it == variables.end()) {
    parsedEquation.pop_back();
    throw runtime_error("Variable/Function " + var + " is not defined");
  }
  parsedEquation.back().updateVariable(var);
}

void Parser::readNumbersVariablesAndFunctions(
    const string &equation, size_t &index,
    const unordered_set<string> &variables) {
  if (!isalnum(equation[index]))
    throw runtime_error(
        "Expected a number, a variable, or a function, but found: " +
        equation.substr(index, 1));
  if (isalpha(equation[index]))
    readVariablesAndFunctions(equation, index, variables);
  else
    readNumbers(equation, index);
}

template <typename Function>
void Parser::pushStackedFunctionsUntil(Function condition) {
  while (!stackedFunctions.empty() &&
         condition(stackedFunctions.top()) == true) {
    parsedEquation.emplace_back();
    parsedEquation.back().updateFunction(stackedFunctions.top());
    stackedFunctions.pop();
  }
}

void Parser::parse(const string &equation,
                   const unordered_set<string> &variables) {
  parsedEquation.clear();
  bool shouldBeOperator = false;
  for (size_t i = 0; i < equation.size();) {
    pushStackedFunctionsUntil([&](const FunctionType &func) -> bool {
      return functions.find(func) != functions.end();
    });
    readWhitespaces(equation, i);
    if (i == equation.size())
      break;
    if (shouldBeOperator) {
      if (equation[i] == ')') {
        pushStackedFunctionsUntil(
            [](const FunctionType &func) -> bool { return func != "("; });
        if (stackedFunctions.empty())
          throw runtime_error("A closing paranthesis doesn't have an opening");
        stackedFunctions.pop();
        i++;
        continue;
      }
      readOperators(equation, i);
    } else {
      readSigns(equation, i);
      if (i >= equation.size())
        throw runtime_error("An equation shouldn't end with an operator");
      if (equation[i] == '(') {
        stackedFunctions.emplace("(");
        i++;
        continue;
      }
      readNumbersVariablesAndFunctions(equation, i, variables);
    }
    shouldBeOperator = !shouldBeOperator;
  }
  if (!shouldBeOperator)
    throw runtime_error(
        "The equation should end with a number, a variable, or a function");
  pushStackedFunctionsUntil(
      [](const FunctionType &func) -> bool { return func != "("; });
  if (!stackedFunctions.empty())
    throw runtime_error("An opening paranthesis doesn't have a closing");
}

const vector<Parser::EquationElement> &Parser::getParsedEquation() const {
  return parsedEquation;
}
