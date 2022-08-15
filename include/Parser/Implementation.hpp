#pragma once

#include "Parser/Interface.hpp"
#include <cstddef>
#include <stack>
#include <unordered_map>

namespace Calculator {

class Parser : public ParserInterface<double, std::string, std::string> {
private:
  using UnionContainer = UnionContainer<double, std::string, std::string>;

  std::unique_ptr<NumberHandlerInterface<double>> reader;
  std::unordered_map<std::string, std::unordered_set<std::string>>
      operatorDependecies;
  std::vector<UnionContainer> parsedEquation;
  std::stack<UnionContainer> stacked;

  void readOperators(const std::string &equation, std::size_t &index);
  void readSigns(const std::string &equation, std::size_t &index);
  void readNumbers(const std::string &equation, std::size_t &index);
  void
  readVariablesAndFunctions(const std::string &equation, std::size_t &index,
                            const std::unordered_set<std::string> &variables);
  void readNumbersVariablesAndFunctions(
      const std::string &equation, std::size_t &index,
      const std::unordered_set<std::string> &variables);

public:
  Parser(std::unique_ptr<NumberHandlerInterface<double>> numberHandler);
  virtual ~Parser() = default;
  virtual void parse(const std::string &equation,
                     const std::unordered_set<std::string> &variables =
                         {}); // throws different exceptions
  virtual const std::vector<UnionContainer> &getParsedEquation() const;
};

} // namespace Calculator
