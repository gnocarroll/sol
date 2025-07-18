#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "macros.h"
#include "operator.h"


class Expr {
public:
    virtual ~Expr() {};

    virtual void print(std::ostream&) = 0;
};

DEF_PTR_TYPES(Expr)

class BinaryExpr final : public Expr {
public:
    const ExprPtr lhs;
    const Operator op;
    const ExprPtr rhs;

    BinaryExpr(ExprPtr&& lhs, Operator op, ExprPtr &&rhs) :
        lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {};

    void print(std::ostream &ostream);
};

class UnaryExpr final : public Expr {
public:

    const Operator op;
    const ExprPtr sub_expr;

    UnaryExpr(Operator op, ExprPtr&& sub_expr) : op(op), sub_expr(std::move(sub_expr)) {}

    void print(std::ostream &ostream);
};

class LiteralExpr : public Expr {
public:
    virtual ~LiteralExpr() {};
};

template <typename T>
class GenericLiteralExpr final : public LiteralExpr {
public:
    const T value;

    GenericLiteralExpr(T&& value) : value(std::move(value)) {}

    void print(std::ostream &ostream) {
        ostream << value;
    }
};

using IntegerLiteralExpr = GenericLiteralExpr<size_t>;
using StringLiteralExpr = GenericLiteralExpr<std::string>;

class ErrExpr final : public Expr {
    void print(std::ostream &ostream) {
        ostream << "ERR";
    }
};