#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <dlfcn.h>

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

class Calculator {
  private:
    double value;
    std::map<std::string, int(*)()> plugins;

    std::string get_plugin_name(std::string path) {
      fs::path fs_path = path;
      return fs_path.stem().string();
    };

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

    void eval(std::string operation, double value) {
      if (operation == "+") {
        this->add(value);
      } else if (operation == "-") {
        this->subtract(value);
      } else if (operation == "*") {
        this->multiply(value);
      } else if (operation == "/") {
        this->divide(value);
      }
    };

    void add_plugin(std::string plugin_path) {
      void* handle = dlopen(plugin_path.c_str(), RTLD_NOW);

      if (handle) {
        void* functionPtr = dlsym(handle, "operation");

        if (functionPtr) {
          std::string operation = this->get_plugin_name(plugin_path);
          this->plugins[operation] = (int(*)())functionPtr;
        }

        dlclose(handle);
      }
    }
};

int main() {
  Calculator calc;

  double init_number;
  std::cout << "Enter a number: ";
  std::cin >> init_number;
  calc.set(init_number);

  while (true) {
    std::string operation;
    std::cout << "Enter an operation: ";
    std::cin >> operation;

    double value;
    std::cout << "Enter the second number: ";
    std::cin >> value;

    calc.eval(operation, value);

    std::cout << "Result is " << calc.get() << std::endl;
    std::cout << std::endl;
  }


  std::cout << "Calculator" << std::endl;


  return 0;
};
