#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <vector>

#include "ast/ast_object.h"
#include "ast/expr.h"
#include "instance.h"
#include "macros.h"
#include "mixins.h"

namespace ast {

struct Statement : public ASTObject {
    Statement() {}
    virtual ~Statement() {}

    Statement(Statement& other) = delete;
};

DEF_DERIVED_TYPES(Statement)

struct CompoundStatement : public Statement {
    std::vector<Statement *> _statements;

    std::vector<Statement*>& statements() {
        return _statements;
    }
};

struct InstanceStatement : public Statement {
    Instance* _instance = nullptr;
    Expr* _expr = nullptr;

    std::optional<Instance*> instance() {
        if (!_instance) return{};

        return _instance;
    }

    std::optional<Expr*> expr() {
        if (!_expr) return {};

        return _expr;
    }
};

/// @brief create new instance of some type
struct CreateStatement final : public InstanceStatement {};

/// @brief modify some instance of a type
struct ModifyStatement final : public InstanceStatement {};

struct PrintStatement final : public Statement {
    std::optional<Expr*> _expr;

    std::optional<Expr*> expr() {
        return _expr;
    }
};

struct ErrStatement final : public Statement {
    ErrStatement() {
        set_err();
    }
};

#undef DECL_STATEMENT_FUNCS

}