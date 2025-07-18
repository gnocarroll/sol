#pragma once

#include "execute.h"
#include "ast/scope.h"
#include "ast/statement.h"

namespace ast {

class Program {
    Scope global_scope;
    CompoundStatement statements;

public:
    Program(Scope&& global_scope, CompoundStatement&& statements) :
        global_scope(global_scope), statements(std::move(statements)) {}

    RetCode execute() {
        return statements.execute();
    }
};

}