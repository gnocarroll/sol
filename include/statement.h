#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <vector>

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
public:
    CreateStatement() {}
    
    DECL_STATEMENT_FUNCS
};

/// @brief modify some instance of a type
class ModifyStatement final : public Statement {
public:
    ModifyStatement() {}

    DECL_STATEMENT_FUNCS
};

#undef DECL_STATEMENT_FUNCS