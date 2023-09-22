#include <cmath>
#include <iostream>
#include <string>

extern "C" {
  double arity = 2;
  std::string sym = "**";

  double eval(double a, double b) {
    return std::pow(a, b);
  }
}
