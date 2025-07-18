#include "parse_program.h"

#include "parse_statement.h"

Program parse_program(CharStream &cstream) {
    Scope global_scope;
    CompoundStatement statements;

    while (true) {
        auto statement = parse_statement(cstream, global_scope);

        if (!statement) break;

        statements.push(std::move(*statement));
    }

    return Program(
        std::move(global_scope),
        std::move(statements)
    );
}