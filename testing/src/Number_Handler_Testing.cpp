#include "Number_Handler/Implementation.hpp"
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>

struct F : public Calculator::NumberHandler {
  F() {}
  ~F() {}
};

using std::runtime_error;

BOOST_FIXTURE_TEST_SUITE(Number_Handler, F)

#define TOLERANCE 1e-6

BOOST_AUTO_TEST_SUITE(fromString_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(fromString("1.1011000280106"), 1.1011000280106, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(fromString("1.225.35625"), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_THROW(fromString("12,456"), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(toString_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(fromString(toString(1.1011000280106)), 1.1011000280106,
                    TOLERANCE);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(add_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(add(1.2, 3.05), 4.25, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_CLOSE(add(1.1011000280106, 3.007080501201), 4.1081805292116,
                    TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_THROW(add(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(add(1, NAN), runtime_error);
  BOOST_CHECK_THROW(add(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(subtract_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(subtract(1.2, 3.05), -1.85, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_CLOSE(subtract(3.1011000280106, 3.007080501201), 0.0940195268096,
                    TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_CLOSE(subtract(1e-6, 2e-6), -1e-6, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case4) {
  BOOST_CHECK_THROW(subtract(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(subtract(1, NAN), runtime_error);
  BOOST_CHECK_THROW(subtract(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(multiply_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(multiply(1.000001, 2.000001), 2.000003000001, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(multiply(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(multiply(1, NAN), runtime_error);
  BOOST_CHECK_THROW(multiply(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(divide_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(divide(1.000001, 2.000001), 0.500000249999, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(divide(1e308, 1e-308), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_THROW(divide(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(divide(1, NAN), runtime_error);
  BOOST_CHECK_THROW(divide(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(power_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(power(2.0001, 4), 16.0032002400080001, TOLERANCE);
  BOOST_CHECK_CLOSE(power(2.0001, -2), 0.249975000625, TOLERANCE);
  BOOST_CHECK_CLOSE(power(-10, 3), -1e3, TOLERANCE);
  BOOST_CHECK_CLOSE(power(-10, -4), 1e-4, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_CLOSE(power(4, 0.5), 2, TOLERANCE);
  BOOST_CHECK_CLOSE(power(4, -0.5), 0.5, TOLERANCE);
  BOOST_CHECK_THROW(power(-4, 0.5), runtime_error);
  BOOST_CHECK_THROW(power(-4, -0.5), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_CLOSE(power(32, 0.2), 2, TOLERANCE);
  BOOST_CHECK_CLOSE(power(32, -0.2), 0.5, TOLERANCE);
  BOOST_CHECK_THROW(power(-32, 0.2), runtime_error);
  BOOST_CHECK_THROW(power(-32, -0.2), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case4) {
  BOOST_CHECK_THROW(power(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(power(1, NAN), runtime_error);
  BOOST_CHECK_THROW(power(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modulo_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(modulo(2.000001, 1.000001), 1, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(modulo(1e308, 0), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_THROW(modulo(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(modulo(1, NAN), runtime_error);
  BOOST_CHECK_THROW(modulo(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(negate_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(negate(21554.5615100541401), -21554.5615100541401,
                    TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(negate(NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(log_Tests)
BOOST_AUTO_TEST_CASE(test_case1) {
  BOOST_CHECK_CLOSE(log(2, 2), 1, TOLERANCE);
  BOOST_CHECK_CLOSE(log(2.1, 2.1), 1, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case2) {
  BOOST_CHECK_THROW(log(-1e20, 10), runtime_error);
  BOOST_CHECK_THROW(log(5, -1), runtime_error);
  BOOST_CHECK_THROW(log(5, 1), runtime_error);
  BOOST_CHECK_THROW(log(-1e10, -10), runtime_error);
  BOOST_CHECK_THROW(log(-3, 3), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case3) {
  BOOST_CHECK_CLOSE(log(1e100, 10), 100, TOLERANCE);
  BOOST_CHECK_CLOSE(log(1e-3, 10), -3, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case4) {
  BOOST_CHECK_CLOSE(log(1e4, -10), 4, TOLERANCE);
  BOOST_CHECK_CLOSE(log(1e-4, -10), -4, TOLERANCE);
  BOOST_CHECK_CLOSE(log(-1e3, -10), 3, TOLERANCE);
  BOOST_CHECK_CLOSE(log(-1e-3, -10), -3, TOLERANCE);
}

BOOST_AUTO_TEST_CASE(test_case5) {
  BOOST_CHECK_THROW(log(-2, -32), runtime_error);
  BOOST_CHECK_CLOSE(log(-32, -2), 5, TOLERANCE);
  BOOST_CHECK_CLOSE(log(4, -2), 2, TOLERANCE);
  BOOST_CHECK_THROW(log(-4, -2), runtime_error);
}

BOOST_AUTO_TEST_CASE(test_case6) {
  BOOST_CHECK_THROW(log(NAN, 1), runtime_error);
  BOOST_CHECK_THROW(log(1, NAN), runtime_error);
  BOOST_CHECK_THROW(log(NAN, NAN), runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()

#undef TOLERANCE

BOOST_AUTO_TEST_SUITE_END()
