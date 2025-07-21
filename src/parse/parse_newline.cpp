#include "parse.h"

#include "test_token.h"

namespace parse {

void parse_zero_plus_newlines(ast::AST& ast_builder) {
    if (match_token(ast_builder.cstream, TokenType::TOK_EOF)) return;

    while (match_token(ast_builder.cstream, TokenType::TOK_NEWLINE));
}

bool parse_one_plus_newlines(ast::AST& ast_builder) {
    if (match_token(ast_builder.cstream, TokenType::TOK_EOF)) return true;
    if (!match_token(ast_builder.cstream, TokenType::TOK_NEWLINE)) return false;

    parse_zero_plus_newlines(ast_builder);

    return true;
}

}