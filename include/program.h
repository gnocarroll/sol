#pragma once

#include "statement.h"

class Program {
    CompoundStatement statements;
public:
    Program(CompoundStatement&& statements) :
        statements(std::move(statements)) {}

    void execute() {
        statements.execute();
    }
};