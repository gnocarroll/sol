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
    std::vector<std::unique_ptr<Statement> > statements;

public:
    CompoundStatement() {}

    void push(std::unique_ptr<Statement> &&new_statement) {
        statements.emplace_back(std::move(new_statement));

        if (statements.back()->has_err()) {
            set_err();
        }
    }

    DECL_STATEMENT_FUNCS
};

class IfStatement : public Statement {
public:
    class ConditionAndBody {
        ExprPtr condition;
        CompoundStatement if_body;
    };

private:
    std::vector<ConditionAndBody> if_thens;

    std::optional<CompoundStatement> else_body;

public:
    IfStatement(
        std::vector<ConditionAndBody>&& if_thens,
        std::optional<CompoundStatement>&& else_body
    ) : if_thens(std::move(if_thens)), else_body(std::move(else_body)) {}

    DECL_STATEMENT_FUNCS
};

class WhileStatement : public Statement {
    ExprPtr condition;
    CompoundStatement while_body;

public:
    WhileStatement(ExprPtr&& condition, CompoundStatement&& while_body) :
        condition(std::move(condition)), while_body(std::move(while_body)) {}

    DECL_STATEMENT_FUNCS
};

/// @brief create new instance of some type
class CreateStatement final : public Statement {
    Instance& instance;
    OptionalExprPtr expr;

public:
    CreateStatement(Instance& instance) : instance(instance) {}
    CreateStatement(Instance& instance, ExprPtr&& expr) :
        instance(instance), expr(std::move(expr)) {}
    
    DECL_STATEMENT_FUNCS
};

/// @brief modify some instance of a type
class ModifyStatement final : public Statement {
    Instance& instance;
    ExprPtr expr;

public:
    ModifyStatement(Instance &instance, ExprPtr&& expr) :
        instance(instance), expr(std::move(expr)) {}

    DECL_STATEMENT_FUNCS
};

class PrintStatement final : public Statement {
    OptionalExprPtr expr;

public:
    PrintStatement() {}
    PrintStatement(ExprPtr &&expr) :
        expr(std::move(expr)) {}

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