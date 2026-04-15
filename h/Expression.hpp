#pragma once

#include "int128.hpp"
#include <memory>
#include <map>
#include <string>
#include <iostream>

class Expression {
public:
    virtual ~Expression() = default;

    virtual Int128 eval(const std::map<std::string, Int128>& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class Const : public Expression {
private:
    Int128 value_;

public:
    explicit Const(Int128 v);

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Variable : public Expression {
private:
    std::string name_;

public:
    explicit Variable(std::string name);

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Binary : public Expression {
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    Binary(const Expression& l, const Expression& r);
};

class Add : public Binary {
public:
    using Binary::Binary;

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Subtract : public Binary {
public:
    using Binary::Binary;

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Multiply : public Binary {
public:
    using Binary::Binary;

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Divide : public Binary {
public:
    using Binary::Binary;

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Negate : public Expression {
private:
    std::unique_ptr<Expression> expr_;

public:
    explicit Negate(const Expression& e);

    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

Add operator+(const Expression& a, const Expression& b);
Subtract operator-(const Expression& a, const Expression& b);
Multiply operator*(const Expression& a, const Expression& b);
Divide operator/(const Expression& a, const Expression& b);