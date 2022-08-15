#include "Evaluator/Implementation.hpp"
#include "Number_Handler/Implementation.hpp"
#include "Parser/Implementation.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;
int main() {
  Calculator::Parser parser(make_unique<Calculator::NumberHandler>());
  unordered_set<string> vars = {"xyz", "y"};
  unordered_map<string, double> vars2nums;
  vars2nums["xyz"] = 2;
  vars2nums["y"] = 1.5;
  parser.parse("--+-4 ^ ---8 + -2.1 * 2*xyz % y", vars);
  auto eq = parser.getParsedEquation();
  Calculator::Evaluator evaluator(make_unique<Calculator::NumberHandler>());
  cout << evaluator.evaluate(eq, vars2nums) << endl;
  return 0;
}
