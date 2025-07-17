#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <string>

#include "macros.h"
#include "operator.h"


class Expr {
    
};

DEF_PTR_TYPES(Expr)

class BinaryExpr : Expr {
public:
    const ExprPtr lhs;
    const Operator op;
    const ExprPtr rhs;

    BinaryExpr(ExprPtr&& lhs, Operator op, ExprPtr &&rhs) :
        lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {};
};

class UnaryExpr : Expr {
public:

    const Operator op;
    const ExprPtr sub_expr;

    UnaryExpr(Operator op, ExprPtr&& sub_expr) : op(op), sub_expr(std::move(sub_expr)) {}
};

class PrimaryExpr : Expr {
public:
    const ExprPtr sub_expr;

    PrimaryExpr(ExprPtr&& sub_expr) : sub_expr(std::move(sub_expr)) {}
};

class LiteralExpr : Expr {

};

template <typename T>
class GenericLiteralExpr : LiteralExpr {
public:
    const T value;

    GenericLiteralExpr(T&& value) : value(std::move(value)) {}
};

using IntegerLiteralExpr = GenericLiteralExpr<size_t>;
using StringLiteralExpr = GenericLiteralExpr<std::string>;