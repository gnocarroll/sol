#include "parse.h"

namespace parse {

ast::Program parse_program(CharStream &cstream) {
    ast::Scope global_scope;
    ast::CompoundStatement statements;

    while (true) {
        auto statement = parse_statement(cstream, global_scope);

        if (!statement) break;

        statements.push(std::move(*statement));
    }

    return ast::Program(
        std::move(global_scope),
        std::move(statements)
    );
}

}