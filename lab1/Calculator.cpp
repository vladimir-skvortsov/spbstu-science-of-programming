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

double Calculator::eval(const std::string& expression) const {
  std::vector<std::string> tokens = get_tokens(expression);
  double result = execute_tokens(tokens);
  return result;
};

void Calculator::add_plugin(const std::string& filename) {
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
    char* error = dlerror();
    if (error != nullptr) {
      throw std::runtime_error(error);
    }

    op = new Unary_operator(name, sym, associativity, eval);
  } else {
    void* precedence_ptr = dlsym(handler, "precedence");
    int precedence = *reinterpret_cast<int*>(precedence_ptr);
    char* error = dlerror();
    if (error != nullptr) {
      throw std::runtime_error(error);
    }

    op = new Binary_operator(name, sym, precedence, eval);
  }

  operators.push_back(op);
};

Operator* Calculator::get_operator(const std::string& sym) const {
  for (auto& op : operators) {
    if (op->get_sym() == sym) {
      return op;
    }
  }

  throw std::runtime_error("Unexpected operator " + sym);
};

int Calculator::get_precedence(const std::string& sym) const {
  Operator* op = get_operator(sym);
  int arity = get_arity(sym);
  if (op->get_arity() == 2) {
    return dynamic_cast<Binary_operator*>(op)->get_precedence();
  } else {
    return 1000;
  }
};

int Calculator::get_arity(const std::string& sym) const {
  Operator* op = get_operator(sym);
  return op->get_arity();
};

bool Calculator::get_associativity(const std::string& sym) const {
  Operator* op = get_operator(sym);
  int arity = get_arity(sym);
  if (arity == 1) {
    return dynamic_cast<Unary_operator *>(op)->get_associativity() == Left_associativity;
  } else {
    return true;
  }
};

template<typename Base, typename T>
inline bool instance_of(const T *ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

bool Calculator::is_operator(const std::string& sym) const {
  if (sym == "(" || sym == ")")
    return false;
  Operator* op = get_operator(sym);
  return instance_of<Unary_operator>(op) || instance_of<Binary_operator>(op);
}

bool Calculator::is_function(const std::string& symbol) const {
  Operator* op = get_operator(symbol);
  return instance_of<Function_operator>(op);
}

std::vector<std::string> Calculator::get_tokens(const std::string& expression) const {
  std::stack<std::string> operators_stack;
  std::vector<std::string> tokens;

  for (int index = 0; index < expression.length(); index += 1) {
    char ch = expression[index];
    std::string current_entity = {ch};

    if (ch == ' ')
      continue;

    if (ch == '(') {
      operators_stack.push(current_entity);
    } else if (ch == ')') {
      bool pe = false;

      while (!operators_stack.empty()) {
        std::string sc = operators_stack.top();
        operators_stack.pop();

        if (sc == "(") {
          pe = true;
          break;
        } else {
          tokens.push_back(sc);
        }
      }

      if (!pe) {
        throw std::runtime_error("Parentheses mismatched");
      }

      if (!operators_stack.empty()) {
        std::string sc = operators_stack.top();

        if (is_function(sc)) {
          tokens.push_back(sc);
          operators_stack.pop();
        }
      }
    } else if (isdigit(ch)) {
      int dots_counter = 0;

      while (index < expression.length()) {
        char ch_number = expression[index + 1];

        if (ch_number == '.') {
          dots_counter += 1;

          if (dots_counter == 2) {
            throw std::runtime_error("More than one dot in number " + current_entity);
          }

          current_entity += ch_number;
        } else if (isdigit(ch_number)) {
          current_entity += ch_number;
        } else {
          break;
        }

        index += 1;
      }

      tokens.push_back(current_entity);
    } else if (isalpha(ch)) {
      while (index < expression.length()) {
        char ch_function = expression[index + 1];

        if (isalpha(ch_function)) {
          current_entity += ch_function;
        } else {
          break;
        }

        index += 1;
      }

      if (is_function(current_entity)) {
        operators_stack.push(current_entity);
      }
    } else if (is_operator(current_entity)) {
      while (!operators_stack.empty()) {
        std::string sc = operators_stack.top();
        std::string sc_str = {sc};

        if (is_operator(sc_str) && ((get_associativity(current_entity) && (get_precedence(current_entity) <= get_precedence(sc_str))) ||
                                    (!get_associativity(current_entity) && (get_precedence(current_entity) < get_precedence(sc_str))))) {
          tokens.push_back(sc_str);
          operators_stack.pop();
        } else {
          break;
        }
      }

      operators_stack.push(current_entity);
    } else {
      throw std::runtime_error("Unknown token in " + current_entity);
    }
  }

  while (!operators_stack.empty()) {
    std::string sc = operators_stack.top();
    operators_stack.pop();

    if (sc == "(" || sc == ")") {
      throw std::runtime_error("Parentheses mismatched");
    }

    tokens.push_back(sc);
  }

  return tokens;
};

double Calculator::execute_tokens(const std::vector<std::string>& tokens) const {
  std::string input = "";
  for (const auto& token : tokens) {
    input += token + "|";
  }

  std::vector<std::string> stack(input.length());
  std::vector<double> stack2(input.length());
  int sl = 0;
  int rn = 0;

  for (int index = 0; index < input.length(); index += 1) {
    char ch = input[index];
    std::string current_entity = {ch};

    if (isdigit(ch)) {
      char ch_current = input[index + 1];

      while (ch_current != '|') {
        current_entity += ch_current;
        index += 1;
        ch_current = input[index + 1];
      }

      index += 1;
      stack[sl] = current_entity;
      stack2[sl] = std::stod(current_entity);
      sl += 1;
    } else {
      if (isalpha(ch)) {
        char ch_current = input[index + 1];

        while (ch_current != '|') {
          current_entity += ch_current;
          index += 1;
          ch_current = input[index + 1];
        }

        stack[sl] = current_entity;
      }

      index += 1;

      if (is_operator(current_entity) || is_function(current_entity)) {
        int arity = get_arity(current_entity);
        double val = 0;
        std::string res = "(" + std::to_string(rn) + ")";
        rn += 1;

        if (sl < arity) {
          throw std::runtime_error("Insufficient arguments in expression");
        }

        if (arity == 1) {
          double arg = stack2[sl - 1];
          sl -= 1;

          Operator* op = get_operator(current_entity);
          val = op->eval({arg});
        } else {
          double left_arg = stack2[sl - 2];
          double right_arg = stack2[sl - 1];

          Operator* op = get_operator(current_entity);
          val = op->eval({left_arg, right_arg});

          sl -= 2;
        }

        stack[sl] = res;
        stack2[sl] = val;
        sl += 1;
      }
    }
  }

  if (sl == 1) {
    return stack2[sl - 1];
  }

  throw std::runtime_error("Too many values");
}
