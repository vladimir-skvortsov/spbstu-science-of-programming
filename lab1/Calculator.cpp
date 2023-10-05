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
  std::string output = "";

  if (shunting_yard(expression, output)) {
    execution_order(output);
  }

  std::cout << output << std::endl;

  return 42;
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

bool Calculator::is_operator(const std::string& symbol) const {
  if (symbol == "(" || symbol == ")")
    return false;
  bool acc = this->get_associativity(symbol);
  int bin = this->get_arity(symbol);
  if ((acc == true && bin == 2) || (acc == false && bin == 1))
    return true;
  return false;
}

bool Calculator::is_function(const std::string& symbol) const {
  if (symbol == "(" || symbol == ")")
    return false;
  if (this->get_associativity(symbol) == true && this->get_arity(symbol) == 1)
    return true;
  return false;
}

bool isIdent(const std::string &symbol)
{
  return (symbol >= "0" && symbol <= "9");
}

bool isIdent(char symbol)
{
  return (symbol >= '0' && symbol <= '9');
}

bool isLetter(char symbol)
{
  return (symbol >= 'a' && symbol <= 'z');
}

bool isLetter(const std::string &symbol)
{
  return (symbol >= "a" && symbol <= "z");
}

bool Calculator::shunting_yard(const std::string& input, std::string& output) const {
  bool interruption = false;
  std::stack<std::string> op_stack;
  int length = input.length();
  output = "";
  for (int i = 0; i < length; ++i)
  {
    if (interruption == true)
    {
      std::cerr << " Process was interrupted" << std::endl;
      std::cerr << " Please, rewrite expression" << std::endl;
      return false;
    }
    char c = input[i];
    std::string c_str = {c};
    if (c_str != " ")
    {
      if (c == '(')
      {
        op_stack.push(c_str);
      }
      else if (c == ')')
      {
        bool pe = false;
        while (!op_stack.empty())
        {
          std::string sc = op_stack.top();
          op_stack.pop();
          if (sc == "(")
          {
            pe = true;
            break;
          }
          else
            output += sc + "|";
        }
        if (!pe)
        {
          std::cout << "Error: parentheses mismatched" << std::endl;
          interruption = true;
          return false;
        }
        if (!op_stack.empty())
        {
          std::string sc = op_stack.top();
          if (is_function(sc))
          {
            output += sc + "|";
            op_stack.pop();
          }
        }
      }
      else if (isIdent(c))
      {
        int j = 0;
        int dot_flag = 0;
        while (j < length)
        {
          j = i + 1;
          char c_current = input[j];
          if (c_current == '.')
          {
            dot_flag++;
            if (dot_flag < 2)
              c_str += c_current;
            else
            {
              std::cerr << " More than one dot in number :" << c_str << std::endl;
              interruption = true;
              break;
            }
          }
          else if (isIdent(c_current))
            c_str += c_current;
          else
            break;
          i++;
        }
        output += c_str + "|";
      }
      else if (isLetter(c_str))
      {
        int j = i + 1;
        char c_current = input[j];
        while (j < length)
        {
          j = i + 1;
          char c_current = input[j];
          if (isLetter(c_current))
            c_str += c_current;
          else
            break;
          i++;
        }
        if (is_function(c_str))
          op_stack.push(c_str);
      }
      else if (is_operator(c_str))
      {
        while (!op_stack.empty())
        {
          std::string sc = op_stack.top();
          std::string sc_str = {sc};
          if (is_operator(sc_str) && ((get_associativity(c_str) && (get_precedence(c_str) <= get_precedence(sc_str))) ||
                                      (!get_associativity(c_str) && (get_precedence(c_str) < get_precedence(sc_str)))))
          {
            output += sc_str + "|";
            op_stack.pop();
          }
          else
            break;
        }
        op_stack.push(c_str);
      }
      else
      {
        std::cout << "Unknown token in" << c_str << std::endl;
        interruption = true;
        return false;
      }
    }
  }
  while (!op_stack.empty())
  {
    std::string sc = op_stack.top();
    op_stack.pop();
    if (sc == "(" || sc == ")")
    {
      std::cout << "Error: parentheses mismatched" << std::endl;
      interruption = true;
      return false;
    }
    output += sc + "|";
  }
  return true;
};

bool Calculator::execution_order(const std::string &input) const {
  int const length = input.length();
  std::vector<std::string> stack(length);
  std::vector<double> stack2(length);
  int sl = 0;
  int rn = 0;
  for (int i = 0; i < length; ++i)
  {
    char c = input[i];
    std::string c_str = {c};
    if (isIdent(c_str))
    {
      double val;
      char c_current = input[i + 1];
      while (c_current != '|')
      {

        c_str += c_current;
        i++;
        c_current = input[i + 1];
      }
      i++;
      std::istringstream(c_str) >> val;
      stack[sl] = c_str;
      stack2[sl] = val;
      ++sl;
    }
    else
    {
      if (isLetter(c_str))
      {
        char c_current = input[i + 1];
        while (c_current != '|')
        {
          c_str += c_current;
          i++;
          c_current = input[i + 1];
        }
        stack[sl] = c_str;
      }
      i++;
      if (is_operator(c_str) || is_function(c_str))
      {
        int nargs = get_arity(c_str);
        int Tnargs = nargs;
        double val = 0;
        std::string res = "(" + std::to_string(rn++) + ")";
        std::cout << res << " = ";

        if (sl < nargs)
        {
          std::cout << "Error: insufficient arguments in expression" << std::endl;
          return false;
        }

        if (nargs == 1)
        {
          std::string sc = stack[sl - 1];
          double sc2 = stack2[sl - 1];
          sl--;
          Operator* op = get_operator(c_str);
          val = op->eval({sc2});
          if (get_associativity(c_str))
            std::cout << c_str << " " << sc << " = " << val << std::endl;
          else
            std::cout << sc << " " << c_str << " = " << val << std::endl;
        }
        else
        {
          std::string sc1 = stack[sl - 2];
          double sc21 = stack2[sl - 2];
          std::cout << sc1 << " " << c << " ";
          std::string sc2 = stack[sl - 1];
          double sc22 = stack2[sl - 1];
          Operator *op = get_operator(c_str);
          val = op->eval({sc21, sc22});
          std::cout << sc2 << " = " << val << std::endl;
          sl -= 2;
        }
        stack[sl] = res;
        stack2[sl] = val;
        ++sl;
      }
    }
  }
  if (sl == 1)
  {
    std::string sc = stack[sl - 1];
    double sc1 = stack2[sl - 1];
    sl--;
    std::cout << "Result : " << sc << " = " << sc1 << std::endl;
    return true;
  }
  std::cout << "Error: too many values entered by the user" << std::endl;
  return false;
}
