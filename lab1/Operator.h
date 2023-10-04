#pragma once
#include <string>
#include <list>

using Eval_func = std::function<double (const std::list<std::string>&)>;

class Operator {
  private:
    std::string name;
    std::string sym;
    int arity;
    int precedence;
    Eval_func& eval;

  public:
    Operator(std::string name, std::string sym, int arity, int precedence, Eval_func& eval) : name(name), sym(sym), arity(arity), precedence(precedence), eval(eval) {};

    virtual std::string get_name() const final;
    virtual std::string get_sym() const final;
    virtual int get_arity() const final;
    virtual int get_precedence() const final;
    virtual double eval(const std::list<std::string> &args) const final;
};

enum Associativity {
  Left_associativity,
  Right_associativity,
};

class Unary_operator : public Operator {
  private:
    Associativity associativity;

  public:
    Unary_operator(std::string name, std::string sym, int arity, int precedence, Associativity associativity, Eval_func& eval): Operator(name, sym, 1, precedence, eval), associativity(associativity) {};

    virtual Associativity get_associativity() const final;
};

class Function_operator : public Operator {};
