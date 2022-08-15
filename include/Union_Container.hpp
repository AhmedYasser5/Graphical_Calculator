#pragma once

#include <string>

namespace Calculator {

template <typename N, typename O = std::string, typename F = std::string>
class UnionContainer {
private:
  union container {
    N num;
    O op;
    F func;
    container();
    ~container();
  };

  container element;

public:
  enum STATE { NOTHING, NUMBER, OPERATOR, FUNCTION };

private:
  STATE state;
  void killAny();
  void copy(const UnionContainer &other);

public:
  UnionContainer();
  ~UnionContainer();
  UnionContainer(const UnionContainer &other);
  UnionContainer &operator=(const UnionContainer &other);
  constexpr const STATE &getState() const;
  constexpr const N &getNumber() const;   // throws std::logic_error
  constexpr const O &getOperator() const; // throws std::logic_error
  constexpr const F &getFunction() const; // throws std::logic_error
  void updateNumber(const N &num);
  void updateOperator(const O &op);
  void updateFunction(const F &func);
};

} // namespace Calculator

#include <stdexcept>

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F>::container::container() {}

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F>::container::~container() {}

template <typename N, typename O, typename F>
void Calculator::UnionContainer<N, O, F>::killAny() {
  if (state == NUMBER)
    element.num.~N();
  else if (state == OPERATOR)
    element.op.~O();
  else if (state == FUNCTION)
    element.func.~F();
  state = NOTHING;
}

template <typename N, typename O, typename F>
void Calculator::UnionContainer<N, O, F>::copy(const UnionContainer &other) {
  state = other.state;
  if (state == NUMBER)
    new (&element.num) N(other.element.num);
  else if (state == OPERATOR)
    new (&element.op) O(other.element.op);
  else if (state == FUNCTION)
    new (&element.func) F(other.element.func);
}

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F>::UnionContainer() : state(NOTHING) {}

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F>::~UnionContainer() {
  killAny();
}

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F>::UnionContainer(
    const UnionContainer &other) {
  copy(other);
}

template <typename N, typename O, typename F>
Calculator::UnionContainer<N, O, F> &
Calculator::UnionContainer<N, O, F>::operator=(const UnionContainer &other) {
  killAny();
  copy(other);
}

template <typename N, typename O, typename F>
constexpr const typename Calculator::UnionContainer<N, O, F>::STATE &
Calculator::UnionContainer<N, O, F>::getState() const {
  return state;
}

template <typename N, typename O, typename F>
constexpr const N &Calculator::UnionContainer<N, O, F>::getNumber() const {
  if (state != NUMBER)
    throw std::logic_error("It is not currently a number... you cannot get it");
  return element.num;
}

template <typename N, typename O, typename F>
constexpr const O &Calculator::UnionContainer<N, O, F>::getOperator() const {
  if (state != OPERATOR)
    throw std::logic_error(
        "It is not currently an operator... you cannot get it");
  return element.op;
}

template <typename N, typename O, typename F>
constexpr const F &Calculator::UnionContainer<N, O, F>::getFunction() const {
  if (state != FUNCTION)
    throw std::logic_error(
        "It is not currently a function... you cannot get it");
  return element.func;
}

template <typename N, typename O, typename F>
void Calculator::UnionContainer<N, O, F>::updateNumber(const N &num) {
  if (state == NUMBER) {
    element.num = num;
    return;
  }
  killAny();
  new (&element.num) N(num);
  state = NUMBER;
}

template <typename N, typename O, typename F>
void Calculator::UnionContainer<N, O, F>::updateOperator(const O &op) {
  if (state == OPERATOR) {
    element.op = op;
    return;
  }
  killAny();
  new (&element.op) O(op);
  state = OPERATOR;
}

template <typename N, typename O, typename F>
void Calculator::UnionContainer<N, O, F>::updateFunction(const F &func) {
  if (state == FUNCTION) {
    element.func = func;
    return;
  }
  killAny();
  new (&element.func) F(func);
  state = FUNCTION;
}
