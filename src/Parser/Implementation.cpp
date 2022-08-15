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

Parser::Parser(std::unique_ptr<NumberHandlerInterface<double>> numberHandler)
    : reader(std::move(numberHandler)) {
  operatorDependecies["+"] = {"+", "-", "*", "/", "%", "^"};
  operatorDependecies["-"] = {"+", "-", "*", "/", "%", "^"};
  operatorDependecies["*"] = {"*", "/", "%", "^"};
  operatorDependecies["/"] = {"*", "/", "%", "^"};
  operatorDependecies["%"] = {"*", "/", "%", "^"};
  operatorDependecies["^"] = {};
}

void Parser::readOperators(const string &equation, size_t &index) {
  string op;
  while (index < equation.size() &&
         operatorDependecies.find(op) == operatorDependecies.end() &&
         !isalnum(equation[index]))
    op += equation[index++];
  --index;
  auto it = operatorDependecies.find(op);
  if (it == operatorDependecies.end())
    throw runtime_error("Unknown operator: " + op);
  auto &dependencies = it->second;
  while (!stacked.empty() &&
         dependencies.find(stacked.top().getOperator()) != dependencies.end()) {
    parsedEquation.emplace_back(stacked.top());
    stacked.pop();
  }
  UnionContainer part;
  part.updateOperator(op);
  stacked.push(part);
}

void Parser::readSigns(const string &equation, size_t &index) {
  char sign = '+';
  while (index < equation.size() &&
         (equation[index] == '+' || equation[index] == '-')) {
    if (sign == equation[index])
      sign = '+';
    else
      sign = '-';
    ++index;
  }
  --index;
  if (sign == '+')
    return;
  UnionContainer part;
  part.updateFunction("-");
  stacked.push(part);
}

void Parser::readNumbers(const string &equation, size_t &index) {
  string num;
  while (index < equation.size() &&
         (isdigit(equation[index]) || equation[index] == '.'))
    num += equation[index++];
  --index;
  UnionContainer part;
  part.updateNumber(reader->fromString(num));
  parsedEquation.emplace_back(part);
}

void Parser::readVariablesAndFunctions(const string &equation, size_t &index,
                                       const unordered_set<string> &variables) {
  string var;
  while (index < equation.size() && isalpha(equation[index]))
    var += equation[index++];
  --index;
  auto it = variables.find(var);
  if (it == variables.end())
    throw runtime_error("Variable " + var + " is not defined");
  UnionContainer part;
  part.updateFunction(var);
  parsedEquation.emplace_back(part);
}

void Parser::readNumbersVariablesAndFunctions(
    const string &equation, size_t &index,
    const unordered_set<string> &variables) {
  readSigns(equation, index);
  ++index;
  if (index >= equation.size())
    throw runtime_error(
        "The equation should end with a number, a variable, or a function");
  if (!isalnum(equation[index]))
    throw runtime_error(
        "Expected a number, a variable, or a function, but found: " +
        equation.substr(index, 1));
  if (isalpha(equation[index]))
    readVariablesAndFunctions(equation, index, variables);
  else
    readNumbers(equation, index);
}

void Parser::parse(const string &equation,
                   const unordered_set<string> &variables) {
  bool shouldBeOperator = false;
  for (size_t i = 0; i < equation.size(); i++) {
    if (std::isblank(equation[i]))
      continue;
    while (!stacked.empty() &&
           stacked.top().getState() == UnionContainer::FUNCTION) {
      parsedEquation.emplace_back(stacked.top());
      stacked.pop();
    }
    if (shouldBeOperator)
      readOperators(equation, i);
    else
      readNumbersVariablesAndFunctions(equation, i, variables);
    shouldBeOperator = !shouldBeOperator;
  }
  if (!shouldBeOperator)
    throw runtime_error(
        "The equation should end with a number, a variable, or a function");
  while (!stacked.empty()) {
    parsedEquation.emplace_back(stacked.top());
    stacked.pop();
  }
}

const vector<Parser::UnionContainer> &Parser::getParsedEquation() const {
  return parsedEquation;
}
