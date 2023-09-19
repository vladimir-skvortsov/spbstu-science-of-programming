#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> dir_stat(std::string path) {
  std::vector<std::string> files = {};

  for (const auto& entry : fs::directory_iterator(path)) {
    if (fs::is_regular_file(entry)) {
      files.push_back(entry.path());
    }
  }

  return files;
};

// TODO: change this mystery type
using operation_func = int;
std::map<std::string, operation_func> load_plugins() {
  std::vector<std::string> plugin_names = dir_stat("./plugins");

  for (const auto& plugin_name : plugin_names) {
    std::cout << plugin_name << std::endl;
  }

  // TODO: return a value
};

class Calculator {
  private:
    double value;

  public:
    Calculator(): value(0) {};

    void set(double value) {
      this->value = value;
    };

    double get() {
      return this->value;
    };

    void add(double value) {
      this->value += value;
    };

    void subtract(double value) {
      this->value -= value;
    };

    void multiply(double value) {
      this->value *= value;
    };

    void divide(double value) {
      this->value /= value;
    };
};

int main() {
  std::cout << "Calculator" << std::endl;

  load_plugins();

  return 0;
};
