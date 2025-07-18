#include "parse_statement.h"

static OptionalStatementPtr _parse_statement(CharStream &cstream, Scope &scope);

OptionalStatementPtr parse_statement(CharStream& cstream, Scope &scope) {
    auto ret = _parse_statement(cstream, scope);

    if (!ret) return {};

    if (!match_token(cstream, TokenType::TOK_NEWLINE)) {
        return std::make_unique<ErrStatement>();
    }

    return ret;
}

static OptionalStatementPtr _parse_statement(CharStream &cstream, Scope &scope) {
    if (match_token(cstream, TokenType::TOK_PRINT)) {
        auto expr = parse_expr(cstream, scope);

        if (!expr) return std::make_unique<PrintStatement>();

        return std::make_unique<PrintStatement>(std::move(*expr));
    }

    auto n_chars = match_token(cstream, TokenType::TOK_WORD);

    if (!n_chars) return {};

    std::string name = *(cstream.last_n_as_str(*n_chars));

    if (match_token(cstream, TokenType::TOK_COLON_EQUAL)) {
        if (scope.get(name)) return std::make_unique<ErrStatement>();

        auto expr = parse_expr(cstream, scope);

        if (!expr) expr = std::make_unique<ErrExpr>();

        scope.push(Instance(std::string(name)));

        return std::make_unique<CreateStatement>(
            *scope.get(name),
            std::move(*expr)
        );
    }
    else if (match_token(cstream, TokenType::TOK_EQUAL)) {
        auto instance = scope.get(name);

        if (!instance) return std::make_unique<ErrStatement>();

        auto expr = parse_expr(cstream, scope);

        if (!expr) expr = std::make_unique<ErrExpr>();

        return std::make_unique<ModifyStatement>(
            *instance,
            std::move(*expr)
        );
    }

    if (scope.get(name)) return std::make_unique<ErrStatement>();

    scope.push(Instance(std::string(name)));

    return std::make_unique<CreateStatement>(
        *scope.get(name)
    );
}
