#include "parse.h"

namespace parse {

ast::Program parse_program(ast::ASTBuilder& ast_builder) {
    ast::Scope global_scope;
    ast::CompoundStatement statements;

    parse_zero_plus_newlines(ast_builder);

    while (true) {
        auto statement = parse_statement(ast_builder, global_scope);

        if (!statement) break;

        statements.push(std::move(*statement));
    }

    parse_zero_plus_newlines(ast_builder);

    return ast::Program(
        std::move(global_scope),
        std::move(statements)
    );
}

}