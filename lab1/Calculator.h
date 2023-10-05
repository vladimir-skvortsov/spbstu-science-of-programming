#pragma once
#include "Operator.h"
#include <string>
#include <exception>
#include <stack>
#include <iostream>
#include <sstream>
#include <dlfcn.h>

class Calculator {
  private:
    inline static const std::string plugins_dir_path = "./plugins/";
    std::vector<Operator*> operators;

    std::vector<std::string> shunting_yard(const std::string& expression) const;
    double execution_order(const std::vector<std::string>& input) const;
    Operator* get_operator(const std::string& sym) const;
    int get_precedence(const std::string& sym) const;
    int get_arity(const std::string& sym) const;
    bool get_associativity(const std::string& sym) const;
    bool is_operator(const std::string& sym) const;
    bool is_function(const std::string& sym) const;

  public:
    Calculator();

    static std::string get_plugins_dir_path();

    virtual double eval(const std::string& expression) const final;
    virtual void add_plugin(const std::string& filename) final;
};
