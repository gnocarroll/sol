#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <vector>

#include "execute.h"
#include "ast/ast_object.h"
#include "ast/expr.h"
#include "file_pos.h"
#include "instance.h"
#include "macros.h"
#include "mixins.h"

namespace ast {

class Statement : public ASTObject {
public:
    virtual ~Statement() {}

    virtual RetCode execute() = 0;
};

#define DECL_STATEMENT_FUNCS \
    RetCode execute();

DEF_PTR_TYPES(Statement)

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

class ErrStatement final : public Statement, public HasErrMsg {
public:
    ErrStatement(std::string&& err_msg, CharStream &cstream, size_t back = 0) :
        HasErrMsg(std::move(err_msg), cstream, back) {

        set_err();
    }

    DECL_STATEMENT_FUNCS
};

#undef DECL_STATEMENT_FUNCS

}