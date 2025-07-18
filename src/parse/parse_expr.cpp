#include "parse/parse_expr.h"

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
    static ast::OptionalExprPtr name (CharStream&, ast::Scope&);

DECL_EXPR_PARSER(parse_binary_expr)
DECL_EXPR_PARSER(parse_unary_expr)
DECL_EXPR_PARSER(parse_primary_expr)
DECL_EXPR_PARSER(parse_literal_expr)

ast::OptionalExprPtr parse_expr(CharStream& cstream, ast::Scope& scope) {
    return parse_binary_expr(cstream, scope);
}

static ast::OptionalExprPtr _parse_binary_expr(
    CharStream& cstream,
    ast::Scope& scope,
    size_t precedence_idx
) {
    auto parse_sub_expr = [&] {
        if (precedence_idx >= N_PRECEDENCE_LEVELS - 1) return parse_unary_expr(cstream, scope);

        return _parse_binary_expr(cstream, scope, precedence_idx + 1);
    };

    ast::OptionalExprPtr lhs = parse_sub_expr();

    if (!lhs) return {};

    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : binary_ops[precedence_idx]) {
            if (match_token(cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        ast::OptionalExprPtr rhs = parse_sub_expr();

        if (!rhs) {
            rhs = std::make_unique<ast::ErrExpr>();
        }

        lhs = std::make_unique<ast::BinaryExpr>(
            std::move(*lhs),
            *found_op,
            std::move(*rhs)
        );
    }

    return lhs;
}

static ast::OptionalExprPtr parse_binary_expr(CharStream& cstream, ast::Scope& scope) {
    return _parse_binary_expr(cstream, scope, 0);
}

static ast::OptionalExprPtr parse_unary_expr(CharStream& cstream, ast::Scope& scope) {
    std::vector<Operator> op_vector;
    
    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : unary_ops) {
            if (match_token(cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        op_vector.push_back(*found_op);
    }

    auto ret = parse_primary_expr(cstream, scope);

    if (op_vector.size() == 0 && !ret) return {};
    if (!ret) {
        ret = std::make_unique<ast::ErrExpr>();
    }

    for (long long op_idx = op_vector.size() - 1; op_idx >= 0; op_idx--) {
        ret = std::make_unique<ast::UnaryExpr>(
            op_vector[op_idx],
            std::move(*ret)
        );
    }

    return ret;
}

static ast::OptionalExprPtr parse_primary_expr(CharStream& cstream, ast::Scope& scope) {
    // ( expr )

    if (match_token(cstream, TokenType::TOK_L_PAREN)) {

        auto ret = parse_expr(cstream, scope);

        if (!match_token(cstream, TokenType::TOK_R_PAREN)) {
            // TODO: report error, expected R_PAREN
        }

        return ret;
    }
    
    // word

    if (auto n_chars = match_token(cstream, TokenType::TOK_WORD)) {
        std::string name = cstream.last_n_as_str(*n_chars)->get_str();

        auto instance = scope.get(name);

        if (!instance) return std::make_unique<ast::ErrExpr>();

        return std::make_unique<ast::InstanceExpr>(*instance);
    }

    // literal

    return parse_literal_expr(cstream, scope);
}

static ast::OptionalExprPtr parse_literal_expr(CharStream& cstream, ast::Scope& scope) {
    auto n_chars = match_token(cstream, TokenType::TOK_INTEGER);

    if (!n_chars) return {};

    auto str = cstream.last_n_as_str(*n_chars)->get_str();

    return std::make_unique<ast::IntegerLiteralExpr>(std::strtoull(
        str.data(),
        nullptr,
        0 // make use of provided feature to autodetect base
    ));
}

}