#pragma once

#include <expected>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "ast/ast_builder.h"
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
    const Expr& lhs;
    const Operator op;
    const Expr& rhs;

    BinaryExpr(ast::AST& ast_builder, const Expr& _lhs, Operator op, const Expr&&&_rhs) :
        lhs(_lhs), op(op), rhs(_rhs) {
        
        if (lhs.has_err() || rhs.has_err()) {
            set_err();
        }

        const auto& lhs_type = lhs.get_lang_type();
        const auto& rhs_type = rhs.get_lang_type();

        if (&lhs_type != &rhs_type) return;

        Value::set_lang_type(
            lhs_type.get_binary_op_ret_type(op)
        );

        if (&get_lang_type() == &lang_err_type) {
            auto optional_op_text = op.get_text();
            auto op_text = optional_op_text ? *optional_op_text : std::string_view("ERR");

            ast_builder.register_error(
                std::string("binary operation not supported: ") +
                std::string(lhs_type.get_name()) +
                " " +
                std::string(op_text) +
                " " +
                std::string(rhs_type.get_name())
            );
        }
    };

    DECL_EXPR_FUNCS
};

class UnaryExpr final : public Expr {
public:

    const Operator op;
    const Expr& sub_expr;

    UnaryExpr(ast::AST& ast_builder, Operator op, const Expr& _sub_expr) :
        op(op), sub_expr(_sub_expr) {
        if (sub_expr.has_err()) {
            set_err();
        }

        const auto& sub_expr_type = sub_expr.get_lang_type();

        Value::set_lang_type(
            sub_expr_type.get_unary_op_ret_type(op)
        );

        if (&get_lang_type() == &lang_err_type) {
            auto optional_op_text = op.get_text();
            auto op_text = optional_op_text ? *optional_op_text : std::string_view("ERR");

            ast_builder.register_error(
                std::string("unary operation not supported: ") +
                std::string(op_text) +
                " " +
                std::string(sub_expr_type.get_name())
            );
        }
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
    const Instance &instance;

public:
    InstanceExpr(const Instance &instance) : instance(instance) {
        Value::set_lang_type(instance.get_lang_type());
    }

    DECL_EXPR_FUNCS
};

class ErrExpr final : public Expr {
public:
    ErrExpr() {
        set_err();

        Value::set_lang_type(lang_err_type);
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