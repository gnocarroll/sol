#include "parse.h"

#include <cstdlib>
#include <initializer_list>

#include "operator.h"
#include "test_token.h"
#include "token.h"

#define N_PRECEDENCE_LEVELS (3)

namespace parse {

/// @brief Grouped by precedence, later => higher precedence
static std::initializer_list<Operator> binary_ops[N_PRECEDENCE_LEVELS] = {
    {Operator::OP_ADD, Operator::OP_SUB},
    {Operator::OP_MULT, Operator::OP_DIV, Operator::OP_MOD},
    {Operator::OP_POW}
};

static Operator unary_ops[] = {
    Operator::OP_ADD,
    Operator::OP_SUB
};

#define DECL_EXPR_PARSER(name) \
    static ast::OptionalExprPtr name (ast::ASTBuilder&, ast::Scope&);

DECL_EXPR_PARSER(parse_binary_expr)
DECL_EXPR_PARSER(parse_unary_expr)
DECL_EXPR_PARSER(parse_primary_expr)
DECL_EXPR_PARSER(parse_literal_expr)

ast::OptionalExprPtr parse_expr(ast::ASTBuilder& ast_builder, ast::Scope& scope) {
    return parse_binary_expr(ast_builder, scope);
}

static ast::OptionalExprPtr _parse_binary_expr(
    ast::ASTBuilder& ast_builder,
    ast::Scope& scope,
    size_t precedence_idx
) {
    auto parse_sub_expr = [&] {
        if (precedence_idx >= N_PRECEDENCE_LEVELS - 1) return parse_unary_expr(ast_builder, scope);

        return _parse_binary_expr(ast_builder, scope, precedence_idx + 1);
    };

    ast::OptionalExprPtr lhs = parse_sub_expr();

    if (!lhs) return {};

    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : binary_ops[precedence_idx]) {
            if (match_token(ast_builder.cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        ast::OptionalExprPtr rhs = parse_sub_expr();

        if (!rhs) {
            ast_builder.register_error("found no rhs for binary expr");

            rhs = ast_builder.make_w_pos<ast::ErrExpr>();
        }

        lhs = std::make_unique<ast::BinaryExpr>(
            std::move(*lhs),
            *found_op,
            std::move(*rhs)
        );
    }

    return lhs;
}

static ast::OptionalExprPtr parse_binary_expr(ast::ASTBuilder& ast_builder, ast::Scope& scope) {
    return _parse_binary_expr(ast_builder, scope, 0);
}

static ast::OptionalExprPtr parse_unary_expr(ast::ASTBuilder& ast_builder, ast::Scope& scope) {
    std::vector<Operator> op_vector;
    
    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : unary_ops) {
            if (match_token(ast_builder.cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        op_vector.push_back(*found_op);
    }

    auto ret = parse_primary_expr(ast_builder, scope);

    if (op_vector.size() == 0 && !ret) return {};
    if (!ret) {
        ast_builder.register_error("found operators but no operand for unary expr");

        ret = ast_builder.make_w_pos<ast::ErrExpr>();
    }

    for (long long op_idx = op_vector.size() - 1; op_idx >= 0; op_idx--) {
        ret = std::make_unique<ast::UnaryExpr>(
            op_vector[op_idx],
            std::move(*ret)
        );
    }

    return ret;
}

static ast::OptionalExprPtr parse_primary_expr(ast::ASTBuilder& ast_builder, ast::Scope& scope) {
    // ( expr )

    if (match_token(ast_builder.cstream, TokenType::TOK_L_PAREN)) {

        auto ret = parse_expr(ast_builder, scope);

        if (!match_token(ast_builder.cstream, TokenType::TOK_R_PAREN)) {
            // TODO: report error, expected R_PAREN
        }

        return ret;
    }
    
    // word

    if (auto n_chars = match_token(ast_builder.cstream, TokenType::TOK_WORD)) {
        std::string name = ast_builder.cstream.last_n_as_str(*n_chars)->get_str();

        auto instance = scope.get(name);

        if (!instance) {
            ast_builder.register_error(std::string("unrecognized variable name: ") + name);

            return ast_builder.make_w_pos<ast::ErrExpr>();
        }

        return ast_builder.make_w_pos<ast::InstanceExpr>(*instance);
    }

    // literal

    return parse_literal_expr(ast_builder, scope);
}

static ast::OptionalExprPtr parse_literal_expr(ast::ASTBuilder& ast_builder, ast::Scope& scope) {
    auto n_chars = match_token(ast_builder.cstream, TokenType::TOK_INTEGER);

    if (!n_chars) return {};

    auto str = ast_builder.cstream.last_n_as_str(*n_chars)->get_str();

    return std::make_unique<ast::IntegerLiteralExpr>(std::strtoull(
        str.data(),
        nullptr,
        0 // make use of provided feature to autodetect base
    ));
}

}