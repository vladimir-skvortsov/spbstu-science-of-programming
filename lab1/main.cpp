#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "Calculator.h"

namespace fs = std::filesystem;

std::vector<std::string> get_plugins_filenames(std::string dir_path) {
  std::vector<std::string> files = {};

  for (const auto& entry : fs::directory_iterator(dir_path)) {
    std::string extension = entry.path().extension();

    if (fs::is_regular_file(entry) && extension == ".dylib") {
      files.push_back(entry.path().filename());
    }
  }

  return files;
};

int main() {
  Calculator* calc = new Calculator();

  std::vector<std::string> plugins = get_plugins_filenames(Calculator::get_plugins_dir_path());
  for (const auto& plugin : plugins) {
    calc->add_plugin(plugin);
  }

  while (true) {
    std::string expression;
    std::cout << "Enter expresion (x for exit): ";
    std::getline(std::cin, expression);

    if (expression == "x") {
      break;
    }

    try {
      double result = calc->eval(expression);
      std::cout << "Result is: " << result << std::endl << std::endl;
    } catch(const std::exception& e) {
      std::cerr << e.what() << std::endl << std::endl;
    }
  }

  return 0;
};
