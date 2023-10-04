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
double Operator::eval(const std::vector<std::string>& args) const {
  return this->eval(args);
};

Associativity Unary_operator::get_associativity() const {
  return associativity;
};

int Binary_operator::get_precedence() const {
  return precedence;
};
