#pragma once
#include "Operator.h"
#include <string>
#include <exception>
#include <dlfcn.h>

class Calculator {
  private:
    inline static const std::string plugins_dir_path = "./plugins/";
    std::vector<Operator*> operators;

  public:
    Calculator();

    static std::string get_plugins_dir_path();

    virtual double eval(const std::string& expression) const final;
    virtual void add_plugin(const std::string& filename) final;
};
