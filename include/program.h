#pragma once

#include "scope.h"
#include "statement.h"

class Program {
    Scope global_scope;
    CompoundStatement statements;

public:
    Program(Scope&& global_scope, CompoundStatement&& statements) :
        global_scope(global_scope), statements(std::move(statements)) {}

    void execute() {
        statements.execute();
    }
};