#pragma once

#include "Parser/Interface.hpp"
#include <cstddef>
#include <stack>
#include <unordered_map>

namespace Calculator {

using std::size_t;
using std::stack;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class Parser : public ParserInterface<double, string, string> {
protected:
  using NumberType = double;
  using VariableType = string;
  using FunctionType = string;
  using EquationElement =
      EquationElement<NumberType, VariableType, FunctionType>;

  unique_ptr<NumberHandlerInterface<NumberType>> reader;
  unordered_map<FunctionType, unordered_set<FunctionType>> operatorDependecies;
  unordered_set<FunctionType> functions;
  vector<EquationElement> parsedEquation;
  stack<FunctionType> stackedFunctions;
  size_t index;
  bool shouldBeOperator;

  template <typename FUNCTION>
  void popStackedFunctionsUntil(FUNCTION &&stopCondition);
  template <typename FUNCTION>
  string readUntil(const string &equation, FUNCTION &&stopCondition);

  void readOperators(const string &equation);
  void readSigns(const string &equation);
  void readNumbers(const string &equation);
  void readComma();
  void readOpeningParanthesis();
  void readClosingParanthesis();

  bool readFunctions(const string &func);
  bool readVariables(const string &var, const unordered_set<string> &variables);
  void readFunctionsAndVariables(const string &equation,
                                 const unordered_set<string> &variables);

public:
  Parser(unique_ptr<NumberHandlerInterface<NumberType>> numberHandler);
  virtual ~Parser() = default;
  virtual void parse(
      const string &equation,
      const unordered_set<string> &variables = {}); // throws std::runtime_error
  virtual const vector<EquationElement> &getParsedEquation() const;
};

} // namespace Calculator
