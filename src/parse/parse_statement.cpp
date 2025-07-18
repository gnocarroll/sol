#include "parse.h"

#include "test_token.h"

namespace parse {

static ast::OptionalStatementPtr _parse_statement(CharStream &cstream, ast::Scope &scope);

ast::OptionalStatementPtr parse_statement(CharStream& cstream, ast::Scope &scope) {
    auto ret = _parse_statement(cstream, scope);

    if (!ret) return {};

    if (!parse_one_plus_newlines(cstream)) {
        return make_unique_w_pos<ast::ErrStatement>(
            cstream.get_file_pos(),
            "expected at least one newline immediately following statement"
        );
    }

    return ret;
}

static ast::OptionalStatementPtr _parse_statement(CharStream &cstream, ast::Scope &scope) {
    if (match_token(cstream, TokenType::TOK_PRINT)) {
        auto expr = parse_expr(cstream, scope);

        if (!expr) return std::make_unique<ast::PrintStatement>();

        return std::make_unique<ast::PrintStatement>(std::move(*expr));
    }

    auto n_chars = match_token(cstream, TokenType::TOK_WORD);

    if (!n_chars) return {};

    std::string name = cstream.last_n_as_str(*n_chars)->get_str();

    if (match_token(cstream, TokenType::TOK_COLON_EQUAL)) {
        if (scope.get(name)) {
            return make_unique_w_pos<ast::ErrStatement>(
                cstream.get_file_pos(),
                std::string("variable already exists: ") + name
            );
        }

        auto expr = parse_expr(cstream, scope);

        if (!expr) expr = make_unique_w_pos<ast::ErrExpr>(
                cstream.get_file_pos(),
                "expected expr to initialize variable"
            );

        scope.push(ast::Instance(std::string(name)));

        return std::make_unique<ast::CreateStatement>(
            *scope.get(name),
            std::move(*expr)
        );
    }
    else if (match_token(cstream, TokenType::TOK_EQUAL)) {
        auto instance = scope.get(name);

        if (!instance) return make_unique_w_pos<ast::ErrStatement>(
                cstream.get_file_pos(),
                std::string("variable name not recognized: ") + name
            );

        auto expr = parse_expr(cstream, scope);

        if (!expr) expr = make_unique_w_pos<ast::ErrExpr>(
                cstream.get_file_pos(),
                std::string("expected expression for use in modifying variable ") + name
            );

        return std::make_unique<ast::ModifyStatement>(
            *instance,
            std::move(*expr)
        );
    }

    if (scope.get(name)) return std::make_unique<ast::ErrStatement>();

    scope.push(ast::Instance(std::string(name)));

    return std::make_unique<ast::CreateStatement>(
        *scope.get(name)
    );
}

}