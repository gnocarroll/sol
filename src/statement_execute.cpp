#include "statement.h"

void CompoundStatement::execute() {
    for (auto& statement : statements) {
        statement->execute();
    }
}