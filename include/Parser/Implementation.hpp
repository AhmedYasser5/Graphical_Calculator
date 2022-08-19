#pragma once

#include "Parser/Interface.hpp"
#include <cstddef>
#include <stack>
#include <unordered_map>

namespace Calculator {

class Parser : public ParserInterface<double, std::string, std::string> {
private:
  using NumberType = double;
  using VariableType = std::string;
  using FunctionType = std::string;
  using EquationElement =
      EquationElement<NumberType, VariableType, FunctionType>;

  std::unique_ptr<NumberHandlerInterface<NumberType>> reader;
  std::unordered_map<FunctionType, std::unordered_set<FunctionType>>
      operatorDependecies;
  std::unordered_set<FunctionType> functions;
  std::vector<EquationElement> parsedEquation;
  std::stack<FunctionType> stackedFunctions;

  template <typename Function>
  void popStackedFunctionsUntil(Function stopCondition);

  void readWhitespaces(const std::string &equation, std::size_t &index) const;

  void readOperators(const std::string &equation, std::size_t &index);
  void readSigns(const std::string &equation, std::size_t &index);
  void readNumbers(const std::string &equation, std::size_t &index);
  std::string readLetters(const std::string &equation, std::size_t &index);

public:
  Parser(std::unique_ptr<NumberHandlerInterface<NumberType>> numberHandler);
  virtual ~Parser() = default;
  virtual void parse(const std::string &equation,
                     const std::unordered_set<std::string> &variables =
                         {}); // throws different exceptions
  virtual const std::vector<EquationElement> &getParsedEquation() const;
};

} // namespace Calculator
