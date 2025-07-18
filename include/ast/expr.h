#pragma once

#include <expected>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "file_pos.h"
#include "instance.h"
#include "macros.h"
#include "operator.h"

namespace ast {

class Expr {
    FilePos file_pos;

public:
    virtual ~Expr() {};

    virtual void print(std::ostream& ostream = std::cout) = 0;
    virtual std::optional<long> eval() = 0;
};

#define DECL_EXPR_FUNCS \
    void print(std::ostream& ostream = std::cout); \
    std::optional<long> eval();

DEF_PTR_TYPES(Expr)

class BinaryExpr final : public Expr {
public:
    const ExprPtr lhs;
    const Operator op;
    const ExprPtr rhs;

    BinaryExpr(ExprPtr&& lhs, Operator op, ExprPtr &&rhs) :
        lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {};

    DECL_EXPR_FUNCS
};

class UnaryExpr final : public Expr {
public:

    const Operator op;
    const ExprPtr sub_expr;

    UnaryExpr(Operator op, ExprPtr&& sub_expr) : op(op), sub_expr(std::move(sub_expr)) {}

    DECL_EXPR_FUNCS
};

class LiteralExpr : public Expr {
public:
    virtual ~LiteralExpr() {};
};

class IntegerLiteralExpr final : public LiteralExpr {
public:
    const long value;

    IntegerLiteralExpr(long value) : value(value) {}

    void print(std::ostream &ostream = std::cout) {
        ostream << value;
    }
    std::optional<long> eval() {
        return value;
    }
};

class InstanceExpr final : public Expr {
    Instance &instance;

public:
    InstanceExpr(Instance &instance) : instance(instance) {}

    DECL_EXPR_FUNCS
};

class ErrExpr final : public Expr {
    void print(std::ostream &ostream = std::cout) {
        ostream << "ERR";
    }
    std::optional<long> eval() {
        // TODO: stop on err
        return {};
    }
};

#undef DECL_EXPR_FUNCS

}