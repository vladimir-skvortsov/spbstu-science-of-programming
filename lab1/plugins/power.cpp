#include <cmath>
#include <iostream>
#include <string>

extern "C" {
  std::string name = "power";
  std::string sym = "^";
  int arity = 2;
  int precedence = 4;
  bool is_function = false;

  double eval(double a, double b) {
    return std::pow(a, b);
  }
}
