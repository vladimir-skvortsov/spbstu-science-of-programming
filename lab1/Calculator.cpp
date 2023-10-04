#include "Operator.h"
#include "Calculator.h"

Calculator::Calculator() {
  Operator* plus_op = new Binary_operator("plus", "+", 2, [] (const std::vector<double>& args) {
    return args[0] + args[1];
  });
  Operator* minus_op = new Binary_operator("minus", "-", 2, [] (const std::vector<double>& args) {
    return args[0] - args[1];
  });
  Operator* multiplication_op = new Binary_operator("multiplication", "*", 3, [] (const std::vector<double>& args) {
    return args[0] * args[1];
  });
  Operator* division_op = new Binary_operator("division", "/", 3, [] (const std::vector<double>& args) {
    return args[0] / args[1];
  });

  operators.push_back(plus_op);
  operators.push_back(minus_op);
  operators.push_back(multiplication_op);
  operators.push_back(division_op);
};

std::string Calculator::get_plugins_dir_path() {
  return Calculator::plugins_dir_path;
};

double Calculator::eval(std::string expression) const {
  return 42;
};

void Calculator::add_plugin(std::string filename) {
  std::string plugin_path = Calculator::plugins_dir_path + filename;
  void* handler = dlopen(plugin_path.c_str(), RTLD_NOW);

  if (!handler) {
    throw std::runtime_error("Could not open plugin at " + plugin_path);
  }

  void* name_ptr = dlsym(handler, "name");
  void* sym_ptr = dlsym(handler, "sym");
  void* arity_ptr = dlsym(handler, "arity");
  void* eval_ptr = dlsym(handler, "eval");
  void* is_function_ptr = dlsym(handler, "is_function");

  char* error = dlerror();

  if (error != nullptr) {
    throw std::runtime_error(error);
  }

  std::string name = reinterpret_cast<char*>(name_ptr);
  std::string sym = reinterpret_cast<char*>(sym_ptr);
  int arity = *reinterpret_cast<int*>(arity_ptr);
  Eval_func eval = reinterpret_cast<double (*)(const std::vector<double> &)>(eval_ptr);
  bool is_function = *reinterpret_cast<bool*>(is_function_ptr);

  Operator* op = nullptr;

  if (is_function) {
    op = new Function_operator(name, sym, arity, eval);
  } else if (arity == 1) {
    void* associativity_ptr = dlsym(handler, "associativity");
    Associativity associativity = *reinterpret_cast<Associativity*>(associativity_ptr);

    op = new Unary_operator(name, sym, associativity, eval);
  } else {
    void* precedence_ptr = dlsym(handler, "precedence");
    int precedence = *reinterpret_cast<int*>(precedence_ptr);

    op = new Binary_operator(name, sym, precedence, eval);
  }

  operators.push_back(op);
};
