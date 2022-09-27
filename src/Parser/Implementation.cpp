#include "Parser/Implementation.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace Calculator;
using std::isalnum;
using std::isalpha;
using std::isblank;
using std::isdigit;
using std::runtime_error;

Parser::Parser(unique_ptr<NumberHandlerInterface<NumberType>> numberHandler)
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

void Parser::emplaceEquationElementNumber(vector<EquationElement> &vec,
                                          const NumberType &num) {
  vec.emplace_back();
  vec.back().updateNumber(num);
}

void Parser::emplaceEquationElementVariable(vector<EquationElement> &vec,
                                            const VariableType &var) {
  vec.emplace_back();
  vec.back().updateVariable(var);
}

void Parser::emplaceEquationElementFunction(vector<EquationElement> &vec,
                                            const FunctionType &func) {
  vec.emplace_back();
  vec.back().updateFunction(func);
}

void Parser::readOperators(const string &equation) {
  popStackedFunctionsUntil([&](const FunctionType &func) -> bool {
    return functions.find(func) == functions.end();
  });

  string op =
      readUntil(equation, [&](const string &op, const char &character) -> bool {
        return operatorDependecies.find(op) != operatorDependecies.end() ||
               isalnum(character);
      });

  auto it = operatorDependecies.find(op);
  if (it == operatorDependecies.end())
    throw runtime_error("Unknown operator: " + op);

  auto &dependencies = it->second;
  popStackedFunctionsUntil([&dependencies](const FunctionType &func) -> bool {
    return dependencies.find(func) == dependencies.end();
  });

  stackedFunctions.emplace(op);
  shouldBeOperator = false;
}

void Parser::readSigns(const string &equation) {
  string signs = readUntil(
      equation, [](const string &signs, const char &character) -> bool {
        return character != '+' && character != '-' && !isblank(character);
      });
  bool areNegativeSignsOdd = std::count(signs.begin(), signs.end(), '-') & 1;
  if (areNegativeSignsOdd)
    stackedFunctions.emplace("neg");
  if (index >= equation.size())
    throw runtime_error("An equation shouldn't end with an operator");
}

void Parser::readNumbers(const string &equation) {
  string num =
      readUntil(equation, [](const string &num, const char &digit) -> bool {
        return !isdigit(digit) && digit != '.';
      });
  emplaceEquationElementNumber(parsedEquation, reader->fromString(num));
  shouldBeOperator = true;
}

void Parser::readComma() {
  index++;
  shouldBeOperator = false;
}

void Parser::readOpeningParanthesis() {
  stackedFunctions.emplace("(");
  index++;
}

void Parser::readClosingParanthesis() {
  popStackedFunctionsUntil(
      [](const FunctionType &func) -> bool { return func == "("; });
  if (stackedFunctions.empty())
    throw runtime_error("A closing paranthesis doesn't have an opening");
  stackedFunctions.pop();
  index++;
}

bool Parser::readFunctions(const string &func) {
  if (functions.find(func) == functions.end())
    return false;
  stackedFunctions.emplace(func);
  return true;
}

bool Parser::readVariables(const string &var,
                           const unordered_set<string> &variables) {
  if (variables.find(var) == variables.end())
    return false;
  emplaceEquationElementVariable(parsedEquation, var);
  shouldBeOperator = true;
  return true;
}

void Parser::readFunctionsAndVariables(const string &equation,
                                       const unordered_set<string> &variables) {
  string word = readUntil(
      equation, [](const string &word, const char &character) -> bool {
        return !isalpha(character);
      });
  bool isRead = readFunctions(word);
  if (isRead)
    return;
  isRead = readVariables(word, variables);
  if (isRead)
    return;
  throw runtime_error("Unknown variable or function: " + word);
}

void Parser::parse(const string &equation,
                   const unordered_set<string> &variables) {
  index = 0;
  parsedEquation.clear();
  shouldBeOperator = false;

  while (index < equation.size()) {
    readUntil(equation, [](const string &str, const char &character) -> bool {
      return !isblank(character);
    });
    if (index >= equation.size())
      break;

    if (shouldBeOperator) {
      if (equation[index] == ',') {
        readComma();
        continue;
      }
      if (equation[index] == ')') {
        readClosingParanthesis();
        continue;
      }
      readOperators(equation);
    } else {
      readSigns(equation);

      if (equation[index] == '(') {
        readOpeningParanthesis();
        continue;
      }

      if (isalpha(equation[index]))
        readFunctionsAndVariables(equation, variables);
      else if (isdigit(equation[index]))
        readNumbers(equation);
      else
        throw runtime_error(
            "Expected a number, a variable, or a function, but found: " +
            equation.substr(index));
    }
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
