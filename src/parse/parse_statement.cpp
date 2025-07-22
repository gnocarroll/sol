#include "parse.h"

#include "test_token.h"

namespace parse {

static ast::OptionalStatementRef _parse_statement(ast::AST& ast, ast::Scope &scope);

ast::OptionalStatementRef parse_statement(ast::AST& ast, ast::Scope &scope) {
    auto ret = _parse_statement(ast, scope);

    if (!ret) return {};

    if (!parse_one_plus_newlines(ast)) {
        ast.register_error("expected at least one newline immediately following statement");

        return ast.make_w_pos<ast::ErrStatement>();
    }

    return ret;
}

static ast::OptionalStatementRef parse_print_statement(ast::AST& ast, ast::Scope& scope) {
    if (!match_token(ast.cstream, TokenType::TOK_PRINT)) return;
    
    auto expr = parse_expr(ast, scope);

    // no expr, will just print newline
    if (!expr) return ast.add_statement(std::make_unique<ast::PrintStatement>());

    // create w/ expr
    return ast.add_statement(std::make_unique<ast::PrintStatement>(*expr));
}

static ast::OptionalStatementRef _parse_statement(ast::AST& ast, ast::Scope &scope) {
    if (auto print_stmt = parse_print_statement(ast, scope)) return print_stmt;

    auto n_chars = match_token(ast.cstream, TokenType::TOK_WORD);

    if (!n_chars) return {};

    std::string name = ast.cstream.last_n_as_str(*n_chars)->get_str();

    if (match_token(ast.cstream, TokenType::TOK_COLON_EQUAL)) {
        if (scope.get(name)) {
            ast.register_error(std::string("variable already exists: ") + name);

            return ast.add_statement(ast.make_w_pos<ast::ErrStatement>());
        }

        auto expr = parse_expr(ast, scope);

        if (!expr) {
            expr = ast.add_expr(ast.make_w_pos<ast::ErrExpr>());
        }

        scope.push(ast::Instance(
            std::string(name),
            (*expr)->get_lang_type()
        ));

        return ast.make_statement<ast::CreateStatement>(
            *scope.get(name),
            std::move(*expr)
        );
    }
    else if (match_token(ast.cstream, TokenType::TOK_EQUAL)) {
        auto instance = scope.get(name);

        if (!instance) {
            ast.register_error(std::string("variable name not recognized: ") + name);

            return ast.make_w_pos<ast::ErrStatement>();
        }

        auto expr = parse_expr(ast, scope);

        if (!expr) {
            ast.register_error(std::string("expected expression for use in modifying variable ") + name);

            expr = ast.make_w_pos<ast::ErrExpr>();
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