#include <string>
#include <vector>
#include <cmath>
#include <exception>

extern "C" {
  std::string name = "ln";
  std::string sym = "ln";
  int arity = 1;
  bool is_function = true;

  double eval(std::vector<double> args) {
    double value = args[0];
    if (value <= 0)
      throw std::invalid_argument("value must be positive");
    return std::log(value);
  }
}
