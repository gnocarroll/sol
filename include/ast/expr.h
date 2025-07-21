#pragma once

#include <expected>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "ast/ast_object.h"
#include "ast/type.h"
#include "file_pos.h"
#include "instance.h"
#include "macros.h"
#include "operator.h"
#include "treewalk.h"

namespace ast {

class Expr : public ASTObject, public Value {
public:
    virtual ~Expr() {};

    virtual void print(std::ostream& ostream = std::cout) = 0;
    virtual treewalk::LiveValuePtr eval(treewalk::ExecutionContext& ctx) const = 0;
};

#define DECL_EXPR_FUNCS \
    void print(std::ostream& ostream = std::cout); \
    treewalk::LiveValuePtr eval(treewalk::ExecutionContext& ctx) const;

DEF_DERIVED_TYPES(Expr)

class BinaryExpr final : public Expr {
public:
    const ExprPtr lhs;
    const Operator op;
    const ExprPtr rhs;

    BinaryExpr(ExprPtr&& _lhs, Operator op, ExprPtr &&_rhs) :
        lhs(std::move(_lhs)), op(op), rhs(std::move(_rhs)) {
        
        if (lhs->has_err() || rhs->has_err()) {
            set_err();
        }

        const auto& lhs_type = lhs->get_lang_type();
        const auto& rhs_type = rhs->get_lang_type();

        if (&lhs_type != &rhs_type) return;

        Value::set_lang_type(
            lhs_type.get_binary_op_ret_type(op)
        );
    };

    DECL_EXPR_FUNCS
};

class UnaryExpr final : public Expr {
public:

    const Operator op;
    const ExprPtr sub_expr;

    UnaryExpr(Operator op, ExprPtr&& _sub_expr) : op(op), sub_expr(std::move(_sub_expr)) {
        if (sub_expr->has_err()) {
            set_err();
        }

        Expr::set_lang_type(
            sub_expr->get_lang_type().get_unary_op_ret_type(op)
        );
    }

    DECL_EXPR_FUNCS
};

class LiteralExpr : public Expr {
public:
    virtual ~LiteralExpr() {};
};

class BooleanLiteralExpr final : public LiteralExpr {
public:
    const bool value;

    BooleanLiteralExpr(bool value) : value(value) {
        Expr::set_lang_type(lang_bool);
    }

    void print(std::ostream &ostream = std::cout) {
        ostream << value;
    }
    treewalk::LiveValuePtr eval(treewalk::ExecutionContext& ctx) const {
        return std::make_unique<treewalk::LiveBooleanValue>(value);
    }
};

class IntegerLiteralExpr final : public LiteralExpr {
public:
    const long value;

    IntegerLiteralExpr(long value) : value(value) {
        Expr::set_lang_type(lang_integer);
    }

    void print(std::ostream &ostream = std::cout) {
        ostream << value;
    }
    treewalk::LiveValuePtr eval(treewalk::ExecutionContext& ctx) const {
        return std::make_unique<treewalk::LiveIntegerValue>(value);
    }
};

class InstanceExpr final : public Expr {
    Instance &instance;

public:
    InstanceExpr(Instance &instance) : instance(instance) {}

    DECL_EXPR_FUNCS
};

class ErrExpr final : public Expr {
public:
    ErrExpr() {
        set_err();
    }

    void print(std::ostream &ostream = std::cout) {
        ostream << "ERR";
    }
    treewalk::LiveValuePtr eval(treewalk::ExecutionContext& ctx) const {
        ctx.register_error(
            *this,
            "attempted to evaluate expression which could not be compiled/constructed correctly"
        );
        return treewalk::LiveErrValue::create();
    }
};

#undef DECL_EXPR_FUNCS

}