#include "parse_statement.h"

OptionalStatementPtr parse_statement(CharStream& cstream) {
    if (match_token(cstream, TokenType::TOK_PRINT)) {
        auto expr = parse_expr(cstream);

        if (!expr) return std::make_unique<PrintStatement>();

        return std::make_unique<PrintStatement>(std::move(*expr));
    }

    
}