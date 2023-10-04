#include "Operator.h"

std::string Operator::get_name() const {
  return name;
};

std::string Operator::get_sym() const {
  return sym;
};

int Operator::get_arity() const {
  return arity;
};

int Operator::get_precedence() const {
  return precedence;
};

double Operator::eval(const std::list<std::string>& args) const {
  return this->eval(args);
};

Associativity Binary_operator::get_associativity() const {
  return associativity;
};
