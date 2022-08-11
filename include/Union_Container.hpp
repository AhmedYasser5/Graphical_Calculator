#pragma once

#include <string>

namespace Calculator {

template <typename T> class NumberOperator {
private:
  union container {
    T num;
    std::string op;
    ~container();
  };

public:
  enum STATE { NUMBER, OPERATOR };

private:
  STATE state;

public:
  ~NumberOperator();
  constexpr STATE getState() const;
  constexpr const T &getNumber() const;             // throws logic_error
  constexpr const std::string &getOperator() const; // throws logic_error
  void updateOperator(const std::string &op);
  void updateNumber(const T &num);
};

} // namespace Calculator
