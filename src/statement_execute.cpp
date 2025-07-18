#include "statement.h"

void CompoundStatement::execute() {
    for (auto& statement : statements) {
        statement->execute();
    }
}

void CreateStatement::execute() {
    if (instance.err || !expr) return;

    auto val = (*expr)->eval();

    if (!val) {
        instance.err = true;
        return;
    }

    instance.value = (*val);
}

void ModifyStatement::execute() {
    if (instance.err) return;

    auto val = expr->eval();

    if (!val) {
        instance.err = true;
        return;
    }

    instance.value = *val;
}

void PrintStatement::execute() {
    if (!expr) {
        std::cout << '\n';
        return;
    }

    auto val = (*expr)->eval();

    if (!val) {
        std::cout << "ERR\n";
        return;
    }

    std::cout << *val << '\n';
}