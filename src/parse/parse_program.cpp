#include "parse.h"

namespace parse {

ast::Program& parse_program(ast::AST& ast) {
    auto& global_scope = ast.make_scope();

    auto& compound_statement = parse_compound_statement(ast, global_scope);

    if (ast.cstream.peekc()) {
        ast.register_error("chars remaining in input stream");
    }

    return ast.make_program(global_scope, compound_statement);
}

}