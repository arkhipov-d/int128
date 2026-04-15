#include "Expression.hpp"

std::ostream& operator<<(std::ostream& os, const Expression& e) {
    e.print(os);
    return os;
}

Const::Const(Int128 v) : value_(v) {}

Int128 Const::eval(const std::map<std::string, Int128>&) const {
    return value_;
}

Expression* Const::clone() const {
    return new Const(value_);
}

void Const::print(std::ostream& os) const {
    os << value_;
}

Variable::Variable(std::string name) : name_(std::move(name)) {}

Int128 Variable::eval(const std::map<std::string, Int128>& vars) const {
    return vars.at(name_);
}

Expression* Variable::clone() const {
    return new Variable(name_);
}

void Variable::print(std::ostream& os) const {
    os << name_;
}

Binary::Binary(const Expression& l, const Expression& r)
    : left_(l.clone()), right_(r.clone()) {
}

Int128 Add::eval(const std::map<std::string, Int128>& vars) const {
    return left_->eval(vars) + right_->eval(vars);
}

Expression* Add::clone() const {
    return new Add(*left_, *right_);
}

void Add::print(std::ostream& os) const {
    os << "(" << *left_ << " + " << *right_ << ")";
}

Int128 Subtract::eval(const std::map<std::string, Int128>& vars) const {
    return left_->eval(vars) - right_->eval(vars);
}

Expression* Subtract::clone() const {
    return new Subtract(*left_, *right_);
}

void Subtract::print(std::ostream& os) const {
    os << "(" << *left_ << " - " << *right_ << ")";
}

Int128 Multiply::eval(const std::map<std::string, Int128>& vars) const {
    return left_->eval(vars) * right_->eval(vars);
}

Expression* Multiply::clone() const {
    return new Multiply(*left_, *right_);
}

void Multiply::print(std::ostream& os) const {
    os << "(" << *left_ << " * " << *right_ << ")";
}

Int128 Divide::eval(const std::map<std::string, Int128>& vars) const {
    return left_->eval(vars) / right_->eval(vars);
}

Expression* Divide::clone() const {
    return new Divide(*left_, *right_);
}

void Divide::print(std::ostream& os) const {
    os << "(" << *left_ << " / " << *right_ << ")";
}

Negate::Negate(const Expression& e)
    : expr_(e.clone()) {
}

Int128 Negate::eval(const std::map<std::string, Int128>& vars) const {
    return -expr_->eval(vars);
}

Expression* Negate::clone() const {
    return new Negate(*expr_);
}

void Negate::print(std::ostream& os) const {
    os << "(-" << *expr_ << ")";
}

Add operator+(const Expression& a, const Expression& b) {
    return Add(a, b);
}

Subtract operator-(const Expression& a, const Expression& b) {
    return Subtract(a, b);
}

Multiply operator*(const Expression& a, const Expression& b) {
    return Multiply(a, b);
}

Divide operator/(const Expression& a, const Expression& b) {
    return Divide(a, b);
}