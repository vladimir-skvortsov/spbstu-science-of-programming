#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

extern "C" {
  std::string name = "sin";
  std::string sym = "sin";
  int arity = 1;
  bool is_function = true;

  double eval(std::vector<double> args) {
    return std::sin(args[0]);
  }
}
