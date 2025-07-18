#include "parse/parse_newline.h"

#include "test_token.h"

namespace parse {

void parse_zero_plus_newlines(CharStream &cstream) {
    if (match_token(cstream, TokenType::TOK_EOF)) return;

    while (match_token(cstream, TokenType::TOK_NEWLINE));
}

bool parse_one_plus_newlines(CharStream &cstream) {
    if (match_token(cstream, TokenType::TOK_EOF)) return true;
    if (!match_token(cstream, TokenType::TOK_NEWLINE)) return false;

    parse_zero_plus_newlines(cstream);

    return true;
}

}