#pragma once

namespace Calculator {

template <typename N, typename V, typename F> class EquationElement {
protected:
  union container {
    N num;
    V var;
    F func;
    container();
    ~container();
  };

  container element;

public:
  enum STATE { NUMBER, VARIABLE, FUNCTION, NOTHING };

protected:
  STATE state;
  void killAny();
  void copy(const EquationElement<N, V, F> &other);

public:
  EquationElement();
  ~EquationElement();
  EquationElement(const EquationElement<N, V, F> &other);
  const EquationElement<N, V, F> &
  operator=(const EquationElement<N, V, F> &other);
  const STATE &getState() const;
  const N &getNumber() const;   // throws std::logic_error
  const V &getVariable() const; // throws std::logic_error
  const F &getFunction() const; // throws std::logic_error
  void updateNumber(const N &num);
  void updateVariable(const V &var);
  void updateFunction(const F &func);
};

} // namespace Calculator

#include <stdexcept>

template <typename N, typename V, typename F>
Calculator::EquationElement<N, V, F>::container::container() {}

template <typename N, typename V, typename F>
Calculator::EquationElement<N, V, F>::container::~container() {}

template <typename N, typename V, typename F>
void Calculator::EquationElement<N, V, F>::killAny() {
  if (state == NUMBER)
    element.num.~N();
  else if (state == VARIABLE)
    element.var.~V();
  else if (state == FUNCTION)
    element.func.~F();
  state = NOTHING;
}

template <typename N, typename V, typename F>
void Calculator::EquationElement<N, V, F>::copy(
    const EquationElement<N, V, F> &other) {
  state = other.state;
  if (state == NUMBER)
    element.num = other.element.num;
  else if (state == FUNCTION)
    element.func = other.element.func;
  else if (state == VARIABLE)
    element.var = other.element.var;
}

template <typename N, typename V, typename F>
Calculator::EquationElement<N, V, F>::EquationElement() : state(NOTHING) {}

template <typename N, typename V, typename F>
Calculator::EquationElement<N, V, F>::~EquationElement() {
  killAny();
}

template <typename N, typename V, typename F>
Calculator::EquationElement<N, V, F>::EquationElement(
    const EquationElement<N, V, F> &other) {
  copy(other);
}

template <typename N, typename V, typename F>
const Calculator::EquationElement<N, V, F> &
Calculator::EquationElement<N, V, F>::operator=(
    const EquationElement<N, V, F> &other) {
  killAny();
  copy(other);
}

template <typename N, typename V, typename F>
const typename Calculator::EquationElement<N, V, F>::STATE &
Calculator::EquationElement<N, V, F>::getState() const {
  return state;
}

template <typename N, typename V, typename F>
const N &Calculator::EquationElement<N, V, F>::getNumber() const {
  if (state != NUMBER)
    throw std::logic_error("It is not currently a number");
  return element.num;
}

template <typename N, typename V, typename F>
const V &Calculator::EquationElement<N, V, F>::getVariable() const {
  if (state != VARIABLE)
    throw std::logic_error("It is not currently a variable");
  return element.var;
}

template <typename N, typename V, typename F>
const F &Calculator::EquationElement<N, V, F>::getFunction() const {
  if (state != FUNCTION)
    throw std::logic_error("It is not currently a function");
  return element.func;
}

template <typename N, typename V, typename F>
void Calculator::EquationElement<N, V, F>::updateNumber(const N &num) {
  if (state == NUMBER) {
    element.num = num;
    return;
  }
  killAny();
  new (&element.num) N(num);
  state = NUMBER;
}

template <typename N, typename V, typename F>
void Calculator::EquationElement<N, V, F>::updateVariable(const V &var) {
  if (state == VARIABLE) {
    element.var = var;
    return;
  }
  killAny();
  new (&element.var) V(var);
  state = VARIABLE;
}

template <typename N, typename V, typename F>
void Calculator::EquationElement<N, V, F>::updateFunction(const F &func) {
  if (state == FUNCTION) {
    element.func = func;
    return;
  }
  killAny();
  new (&element.func) F(func);
  state = FUNCTION;
}
