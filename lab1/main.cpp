#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "Calculator.h"

namespace fs = std::filesystem;

std::vector<std::string> get_plugins_filenames(std::string dir_path) {
  std::vector<std::string> files = {};

  for (const auto& entry : fs::directory_iterator(dir_path)) {
    if (fs::is_regular_file(entry) && entry.path().extension() == ".dylib") {
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

  // double init_number;
  // std::cout << "Enter a number: ";
  // std::cin >> init_number;
  // calc->set(init_number);

  // while (true) {
  //   std::string operation;
  //   std::cout << "Enter an operation (+, -, *, /, x (exit)): ";
  //   std::cin >> operation;

  //   if (operation == "x") {
  //     break;
  //   }

  //   double value;
  //   std::cout << "Enter the second number: ";
  //   std::cin >> value;

  //   calc.eval(operation, value);

  //   std::cout << "Result is " << calc.get() << std::endl;
  //   std::cout << std::endl;
  // }

  return 0;
};
