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
  functions.insert("log");
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

string Parser::readLetters(const string &equation, size_t &index) {
  string word;
  while (index < equation.size() && isalpha(equation[index]))
    word += equation[index++];
  return word;
}

template <typename Function>
void Parser::popStackedFunctionsUntil(Function stopCondition) {
  while (!stackedFunctions.empty() && !stopCondition(stackedFunctions.top())) {
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
    readWhitespaces(equation, i);
    if (i == equation.size())
      break;
    if (shouldBeOperator) {
      if (equation[i] == ',') {
        shouldBeOperator = false;
        i++;
        continue;
      }
      if (equation[i] == ')') {
        popStackedFunctionsUntil(
            [](const FunctionType &func) -> bool { return func == "("; });
        if (stackedFunctions.empty())
          throw runtime_error("A closing paranthesis doesn't have an opening");
        stackedFunctions.pop();
        i++;
        continue;
      }
      popStackedFunctionsUntil([&](const FunctionType &func) -> bool {
        return functions.find(func) == functions.end();
      });
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
      if (isalpha(equation[i])) {
        string word = readLetters(equation, i);
        if (functions.find(word) != functions.end()) {
          stackedFunctions.emplace(word);
          continue;
        } else if (variables.find(word) != variables.end()) {
          parsedEquation.emplace_back();
          parsedEquation.back().updateVariable(word);
        } else
          throw runtime_error("Unknown variable/function " + word);
      } else if (isdigit(equation[i]))
        readNumbers(equation, i);
      else
        throw runtime_error(
            "Expected a number, a variable, or a function, but found: " +
            equation.substr(i));
    }
    shouldBeOperator = !shouldBeOperator;
  }
  if (!shouldBeOperator)
    throw runtime_error(
        "The equation should end with a number, a variable, or a function");
  popStackedFunctionsUntil(
      [](const FunctionType &func) -> bool { return func == "("; });
  if (!stackedFunctions.empty())
    throw runtime_error("An opening paranthesis doesn't have a closing");
}

const vector<Parser::EquationElement> &Parser::getParsedEquation() const {
  return parsedEquation;
}
