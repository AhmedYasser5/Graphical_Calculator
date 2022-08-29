#include "Equation_Element.hpp"
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_SUITE(Equation_Element)

template <typename T> struct TypeParseTraits;

#define REGISTER_PARSE_TYPE(X)                                                 \
  template <> struct TypeParseTraits<X> { static const char *name; };          \
  const char *TypeParseTraits<X>::name = #X

REGISTER_PARSE_TYPE(int);
REGISTER_PARSE_TYPE(float);
REGISTER_PARSE_TYPE(double);
REGISTER_PARSE_TYPE(long double);
REGISTER_PARSE_TYPE(std::size_t);
REGISTER_PARSE_TYPE(std::string);

int t1 = 0;
float t2 = 0;
double t3 = 0;
long double t4 = 0;
std::size_t t5 = 0;
std::string t6 = "";
auto test_types = boost::tie(t1, t2, t3, t4, t5, t6);

template <typename TUPLE, typename FUNCTION>
void chooseTriples(const TUPLE &types, FUNCTION &&testFunction) {
  boost::fusion::for_each(types, [&](auto i) -> void {
    boost::fusion::for_each(types, [&](auto j) -> void {
      boost::fusion::for_each(types, [&](auto k) -> void {
        BOOST_TEST_CHECKPOINT(
            "Number Type: "
            << TypeParseTraits<decltype(i)>::name
            << ", Variable Type: " << TypeParseTraits<decltype(j)>::name
            << ", Function Type: " << TypeParseTraits<decltype(k)>::name);
        testFunction(i, j, k,
                     Calculator::EquationElement<decltype(i), decltype(j),
                                                 decltype(k)>());
      });
    });
  });
}

BOOST_AUTO_TEST_CASE(Numbers_Tests) {
  chooseTriples(
      test_types, [&](auto &&N, auto &&V, auto &&F, auto element) -> void {
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::NOTHING);
        element.updateNumber(N);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::NUMBER);
        BOOST_CHECK_EQUAL(element.getNumber(), N);
        element.updateVariable(V);
        element.updateNumber(N);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::NUMBER);
        BOOST_CHECK_EQUAL(element.getNumber(), N);
        BOOST_CHECK_THROW(element.getVariable(), std::logic_error);
        BOOST_CHECK_THROW(element.getFunction(), std::logic_error);
      });
}

BOOST_AUTO_TEST_CASE(Variables_Tests) {
  chooseTriples(
      test_types, [&](auto &&N, auto &&V, auto &&F, auto element) -> void {
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::NOTHING);
        element.updateVariable(V);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::VARIABLE);
        BOOST_CHECK_EQUAL(element.getVariable(), V);
        element.updateFunction(F);
        element.updateVariable(V);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::VARIABLE);
        BOOST_CHECK_EQUAL(element.getVariable(), V);
        BOOST_CHECK_THROW(element.getNumber(), std::logic_error);
        BOOST_CHECK_THROW(element.getFunction(), std::logic_error);
      });
}

BOOST_AUTO_TEST_CASE(Functions_Tests) {
  chooseTriples(
      test_types, [&](auto &&N, auto &&V, auto &&F, auto element) -> void {
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::NOTHING);
        element.updateFunction(F);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::FUNCTION);
        BOOST_CHECK_EQUAL(element.getFunction(), F);
        element.updateNumber(N);
        element.updateFunction(F);
        BOOST_CHECK_EQUAL(element.getState(), decltype(element)::FUNCTION);
        BOOST_CHECK_EQUAL(element.getFunction(), F);
        BOOST_CHECK_THROW(element.getNumber(), std::logic_error);
        BOOST_CHECK_THROW(element.getVariable(), std::logic_error);
      });
}
BOOST_AUTO_TEST_SUITE_END()
