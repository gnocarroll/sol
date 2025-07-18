#pragma once

#include "execute.h"
#include "scope.h"
#include "statement.h"

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