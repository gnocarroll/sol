#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <vector>

#include "ast/ast_object.h"
#include "ast/expr.h"
#include "file_pos.h"
#include "instance.h"
#include "macros.h"
#include "mixins.h"
#include "treewalk.h"

namespace ast {

class Statement : public ASTObject {
public:
    virtual ~Statement() {}

    virtual void execute(treewalk::ExecutionContext& ctx) = 0;
};

#define DECL_STATEMENT_FUNCS \
    void execute(treewalk::ExecutionContext& ctx);

DEF_DERIVED_TYPES(Statement)

class CompoundStatement : public Statement {
    std::vector<const Statement *> statements;

public:
    CompoundStatement() {}

    void push(const Statement& new_statement) {
        statements.push_back(&new_statement);

        if (statements.back()->has_err()) {
            set_err();
        }
    }

    DECL_STATEMENT_FUNCS
};

/// @brief create new instance of some type
class CreateStatement final : public Statement {
    Instance& instance;
    OptionalExprRef expr;

public:
    CreateStatement(Instance& instance) : instance(instance) {}
    CreateStatement(Instance& instance, Expr& expr) :
        instance(instance), expr(expr) {}
    
    DECL_STATEMENT_FUNCS
};

/// @brief modify some instance of a type
class ModifyStatement final : public Statement {
    Instance& instance;
    Expr& expr;

public:
    ModifyStatement(Instance &instance, Expr& expr) :
        instance(instance), expr(expr) {}

    DECL_STATEMENT_FUNCS
};

class PrintStatement final : public Statement {
    const OptionalExprRef expr;

public:
    PrintStatement() {}
    PrintStatement(Expr& expr) :
        expr(expr) {}

    DECL_STATEMENT_FUNCS
};

class ErrStatement final : public Statement {
public:
    ErrStatement() {
        set_err();
    }

    DECL_STATEMENT_FUNCS
};

#undef DECL_STATEMENT_FUNCS

}