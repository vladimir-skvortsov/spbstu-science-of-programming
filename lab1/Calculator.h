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
    Calculator() {};

    virtual double eval(std::string expression) const final;
    virtual void add_plugin(std::string filename) final;
};
