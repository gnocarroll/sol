#include "parse.h"

#include "test_token.h"

namespace parse {

static ast::OptionalStatementPtr _parse_statement(ast::ASTBuilder& ast_builder, ast::Scope &scope);

ast::OptionalStatementPtr parse_statement(ast::ASTBuilder& ast_builder, ast::Scope &scope) {
    auto ret = _parse_statement(ast_builder, scope);

    if (!ret) return {};

    if (!parse_one_plus_newlines(ast_builder)) {
        ast_builder.register_error("expected at least one newline immediately following statement");

        return ast_builder.make_w_pos<ast::ErrStatement>();
    }

    return ret;
}

static ast::OptionalStatementPtr _parse_statement(ast::ASTBuilder& ast_builder, ast::Scope &scope) {
    if (match_token(ast_builder.cstream, TokenType::TOK_PRINT)) {
        auto expr = parse_expr(ast_builder, scope);

        if (!expr) return std::make_unique<ast::PrintStatement>();

        return std::make_unique<ast::PrintStatement>(std::move(*expr));
    }

    auto n_chars = match_token(ast_builder.cstream, TokenType::TOK_WORD);

    if (!n_chars) return {};

    std::string name = ast_builder.cstream.last_n_as_str(*n_chars)->get_str();

    if (match_token(ast_builder.cstream, TokenType::TOK_COLON_EQUAL)) {
        if (scope.get(name)) {
            ast_builder.register_error(std::string("variable already exists: ") + name);

            return ast_builder.make_w_pos<ast::ErrStatement>();
        }

        auto expr = parse_expr(ast_builder, scope);

        if (!expr) {
            expr = ast_builder.make_w_pos<ast::ErrExpr>();
        }

        scope.push(ast::Instance(std::string(name)));

        return std::make_unique<ast::CreateStatement>(
            *scope.get(name),
            std::move(*expr)
        );
    }
    else if (match_token(ast_builder.cstream, TokenType::TOK_EQUAL)) {
        auto instance = scope.get(name);

        if (!instance) {
            ast_builder.register_error(std::string("variable name not recognized: ") + name);

            return ast_builder.make_w_pos<ast::ErrStatement>();
        }

        auto expr = parse_expr(ast_builder, scope);

        if (!expr) {
            ast_builder.register_error(std::string("expected expression for use in modifying variable ") + name);

            expr = ast_builder.make_w_pos<ast::ErrExpr>();
        }

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