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

std::vector<std::string> plugin_paths_get(std::string dir_path) {
  std::vector<std::string> files = {};

  for (const auto& entry : fs::directory_iterator(dir_path)) {
    if (fs::is_regular_file(entry) && entry.path().extension() == ".dylib") {
      files.push_back(entry.path());
    }
  }

  return files;
};

class Calculator {
  private:
    double value;
    std::map<std::string, int(*)()> plugins;

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
      } else if (this->plugins[operation]) {
        // this->value = this->plugins[operation](this->value, value);
      }
    };

    void add_plugin(std::string plugin_path) {
      void* handle = dlopen(plugin_path.c_str(), RTLD_NOW);

      if (handle) {
        void* func = dlsym(handle, "func");
        void* sym = dlsym(handle, "sym");
          std::cout << "Adding plugin: " << sym << std::endl;

        if (func && sym) {
          char* sym_ptr = reinterpret_cast<char*>(sym);
          std::string sym_str = sym_ptr;
          std::cout << sym_str << std::endl;
          this->plugins[sym_str] = (int(*)())func;
        }

        dlclose(handle);
      }
    }
};

int main() {
  Calculator calc;

  std::vector<std::string> plugins = plugin_paths_get("./plugins");
  for (const auto& plugin : plugins) {
    calc.add_plugin(plugin);
  }

  double init_number;
  std::cout << "Enter a number: ";
  std::cin >> init_number;
  calc.set(init_number);

  while (true) {
    std::string operation;
    std::cout << "Enter an operation (+, -, *, /, x (exit)): ";
    std::cin >> operation;

    if (operation == "x") {
      break;
    }

    double value;
    std::cout << "Enter the second number: ";
    std::cin >> value;

    calc.eval(operation, value);

    std::cout << "Result is " << calc.get() << std::endl;
    std::cout << std::endl;
  }

  return 0;
};
