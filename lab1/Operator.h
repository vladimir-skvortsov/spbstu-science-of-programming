#pragma once
#include <string>
#include <vector>
#include <functional>

using Eval_func = std::function<double (const std::vector<double>&)>;

class Operator {
  private:
    std::string name;
    std::string sym;
    int arity;
    Eval_func eval_func;

  public:
    Operator(std::string name, std::string sym, int arity, Eval_func eval): name(name), sym(sym), arity(arity), eval_func(eval) {};

    virtual std::string get_name() const final;
    virtual std::string get_sym() const final;
    virtual int get_arity() const final;
    virtual double eval(const std::vector<double> &args) const final;
};

enum Associativity {
  Left_associativity,
  Right_associativity,
};

class Unary_operator : public Operator {
  private:
    Associativity associativity;

  public:
    Unary_operator(std::string name, std::string sym, Associativity associativity, Eval_func eval): Operator(name, sym, 1, eval), associativity(associativity) {};

    virtual Associativity get_associativity() const final;
};

class Binary_operator : public Operator {
  private:
    int precedence;

  public:
    Binary_operator(std::string name, std::string sym, int precedence, Eval_func eval): Operator(name, sym, 1, eval), precedence(precedence) {};

    virtual int get_precedence() const final;
};

class Function_operator : public Operator {
  public:
    Function_operator(std::string name, std::string sym, int arity, Eval_func &eval) : Operator(name, sym, arity, eval) {};
};
