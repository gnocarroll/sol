#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <vector>

#include "expr.h"
#include "instance.h"
#include "macros.h"

class Statement {
public:
    virtual ~Statement() {}

    virtual void execute() = 0;
};

#define DECL_STATEMENT_FUNCS \
    void execute();

DEF_PTR_TYPES(Statement)

class CompoundStatement : public Statement {
    std::vector<std::unique_ptr<Statement> > statements;

public:
    CompoundStatement() {}

    void push(std::unique_ptr<Statement> &&new_statement) {
        statements.emplace_back(std::move(new_statement));
    }

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
    ErrStatement() {}

    void execute() {}
};

#undef DECL_STATEMENT_FUNCS