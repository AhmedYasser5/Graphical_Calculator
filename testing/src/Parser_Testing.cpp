#include "Number_Handler/Implementation.hpp"
#include "Parser/Implementation.hpp"
#include <boost/test/unit_test.hpp>

using std::isblank;
using std::make_unique;
using std::size_t;
using std::string;

struct F : public Calculator::Parser {
  F() : Calculator::Parser(make_unique<Calculator::NumberHandler>()) {
    index = 0;
  }
  ~F() {}
};

BOOST_AUTO_TEST_SUITE(Parser)

BOOST_AUTO_TEST_SUITE(popStackedFunctionsUntil_Tests)
BOOST_FIXTURE_TEST_CASE(test_case1, F) {
  stackedFunctions.emplace("log");
  stackedFunctions.emplace("pow");
  stackedFunctions.emplace("(");
  stackedFunctions.emplace(")");
  popStackedFunctionsUntil(
      [](const string &func) -> bool { return func == "pow"; });
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 2);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "pow");
  stackedFunctions.pop();
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "log");
  BOOST_CHECK_EQUAL(parsedEquation.size(), 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(readUntil_Tests)
BOOST_FIXTURE_TEST_CASE(test_case1, F) {
  const string blanks =
      "							"
      "							"
      "							 i";
  readUntil(blanks,
            [](const string &str, const char &c) -> bool { return true; });
  BOOST_CHECK_EQUAL(index, 0);
  readUntil(blanks, [](const string &str, const char &c) -> bool {
    return !isblank(c);
  });
  BOOST_CHECK_EQUAL(index, blanks.size() - 1);
  readUntil(blanks,
            [](const string &str, const char &c) -> bool { return false; });
  BOOST_CHECK_EQUAL(index, blanks.size());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(readOperators_Tests)
BOOST_FIXTURE_TEST_CASE(test_case1, F) {
  shouldBeOperator = true;
  string op = "+	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 1);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "+");
}

BOOST_FIXTURE_TEST_CASE(test_case2, F) {
  shouldBeOperator = true;
  string op = "-	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 1);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "-");
}

BOOST_FIXTURE_TEST_CASE(test_case3, F) {
  shouldBeOperator = true;
  string op = "*	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 3);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "*");
}

BOOST_FIXTURE_TEST_CASE(test_case4, F) {
  shouldBeOperator = true;
  string op = "/	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 3);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "/");
}

BOOST_FIXTURE_TEST_CASE(test_case5, F) {
  shouldBeOperator = true;
  string op = "%	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 3);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "%");
}

BOOST_FIXTURE_TEST_CASE(test_case6, F) {
  shouldBeOperator = true;
  string op = "^	   ";
  stackedFunctions.emplace("+");
  stackedFunctions.emplace("-");
  stackedFunctions.emplace("*");
  stackedFunctions.emplace("/");
  stackedFunctions.emplace("%");
  stackedFunctions.emplace("^");
  stackedFunctions.emplace("neg");
  stackedFunctions.emplace("log");
  readOperators(op);
  BOOST_CHECK_EQUAL(shouldBeOperator, false);
  BOOST_CHECK_EQUAL(index, 1);
  BOOST_CHECK_EQUAL(stackedFunctions.size(), 7);
  BOOST_CHECK_EQUAL(stackedFunctions.top(), "^");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
