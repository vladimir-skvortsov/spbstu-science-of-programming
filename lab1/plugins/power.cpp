#include <string>
#include <vector>
#include <cmath>

extern "C" {
  std::string name = "power";
  std::string sym = "^";
  int arity = 2;
  int precedence = 4;
  bool is_function = false;

  double eval(std::vector<double> args) {
    return std::pow(args[0], args[1]);
  }
}
