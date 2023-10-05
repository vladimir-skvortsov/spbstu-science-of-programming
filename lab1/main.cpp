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

  std::cout << calc->eval("(sin(3.1415926) + 3) ^ 2") << std::endl;

  return 0;
};
