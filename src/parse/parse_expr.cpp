#include "parse.h"

#include <cstdlib>
#include <initializer_list>

#include "operator.h"
#include "test_token.h"
#include "token.h"

#define N_PRECEDENCE_LEVELS (5)

namespace parse {

/// @brief Grouped by precedence, later => higher precedence
static std::initializer_list<Operator> binary_ops[N_PRECEDENCE_LEVELS] = {
    {Operator::OP_EQ, Operator::OP_NEQ},
    {Operator::OP_LT, Operator::OP_GT, Operator::OP_LE, Operator::OP_GE},
    {Operator::OP_ADD, Operator::OP_SUB},
    {Operator::OP_MULT, Operator::OP_DIV, Operator::OP_MOD},
    {Operator::OP_POW}
};

static Operator unary_ops[] = {
    Operator::OP_ADD,
    Operator::OP_SUB,
    Operator::OP_NOT
};

#define DECL_EXPR_PARSER(name) \
    static ast::OptionalExprRef name (ast::AST&, ast::Scope&);

DECL_EXPR_PARSER(parse_binary_expr)
DECL_EXPR_PARSER(parse_unary_expr)
DECL_EXPR_PARSER(parse_primary_expr)
DECL_EXPR_PARSER(parse_literal_expr)

ast::OptionalExprRef parse_expr(ast::AST& ast, ast::Scope& scope) {
    return parse_binary_expr(ast, scope);
}

static ast::OptionalExprRef _parse_binary_expr(
    ast::AST& ast,
    ast::Scope& scope,
    size_t precedence_idx
) {
    auto parse_sub_expr = [&] {
        if (precedence_idx >= N_PRECEDENCE_LEVELS - 1) return parse_unary_expr(ast, scope);

        return _parse_binary_expr(ast, scope, precedence_idx + 1);
    };

    auto lhs = parse_sub_expr();

    if (!lhs) return {};

    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : binary_ops[precedence_idx]) {
            if (match_token(ast.cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        auto rhs = parse_sub_expr();

        if (!rhs) {
            ast.register_error("found no rhs for binary expr");

            rhs = ast.make_err_expr();
        }

        lhs = ast.make_binary_expr(
            *lhs,
            *found_op,
            *rhs
        );
    }

    return lhs;
}

static ast::OptionalExprRef parse_binary_expr(ast::AST& ast, ast::Scope& scope) {
    return _parse_binary_expr(ast, scope, 0);
}

static ast::OptionalExprRef parse_unary_expr(ast::AST& ast, ast::Scope& scope) {
    std::vector<Operator> op_vector;
    
    while (true) {
        std::optional<Operator> found_op;

        for (const auto& op : unary_ops) {
            if (match_token(ast.cstream, *op.get_token_type())) {
                found_op = op;
                break;
            }
        }

        if (!found_op) break;

        op_vector.push_back(*found_op);
    }

    auto ret = parse_primary_expr(ast, scope);

    if (op_vector.size() == 0 && !ret) return {};
    if (!ret) {
        ast.register_error("found operators but no operand for unary expr");

        ret = ast.make_err_expr();
    }

    for (long long op_idx = op_vector.size() - 1; op_idx >= 0; op_idx--) {
        ret = ast.make_unary_expr(
            op_vector[op_idx],
            *ret
        );
    }

    return ret;
}

static ast::OptionalExprRef parse_primary_expr(ast::AST& ast, ast::Scope& scope) {
    // ( expr )

    if (match_token(ast.cstream, TokenType::TOK_L_PAREN)) {

        auto ret = parse_expr(ast, scope);

        if (!match_token(ast.cstream, TokenType::TOK_R_PAREN)) {
            // TODO: report error, expected R_PAREN
        }

        return ret;
    }
    
    // word

    if (auto n_chars = match_token(ast.cstream, TokenType::TOK_WORD)) {
        std::string name = ast.cstream.last_n_as_str(*n_chars)->get_str();

        auto instance = scope.get(name);

        if (!instance) {
            ast.register_error(std::string("unrecognized variable name: ") + name);

            return ast.make_err_expr();
        }

        return ast.make_instance_expr(*instance);
    }

    // literal

    return parse_literal_expr(ast, scope);
}

static ast::OptionalExprRef parse_integer_literal_expr(ast::AST& ast, ast::Scope& scope);
static ast::OptionalExprRef parse_boolean_literal_expr(ast::AST& ast, ast::Scope& scope);

static ast::OptionalExprRef parse_literal_expr(ast::AST& ast, ast::Scope& scope) {
    for (const auto& parse_func : {parse_integer_literal_expr, parse_boolean_literal_expr}) {
        if (const auto& expr = parse_func(ast, scope)) return expr;
    }

    return {};
}

static ast::OptionalExprRef parse_integer_literal_expr(ast::AST& ast, ast::Scope& scope) {
    auto n_chars = match_token(ast.cstream, TokenType::TOK_INTEGER);

    if (!n_chars) return {};

    auto str = ast.cstream.last_n_as_str(*n_chars)->get_str();

    return ast.make_literal_expr(
        ast::lang_integer,
        std::strtoull(
            str.data(),
            nullptr,
            10
        )
    );
}

static ast::OptionalExprRef parse_boolean_literal_expr(ast::AST& ast, ast::Scope& scope) {
    bool value;
    
    if (auto n_chars = match_token(ast.cstream, TokenType::TOK_TRUE)) value = true;
    else if (auto n_chars = match_token(ast.cstream, TokenType::TOK_FALSE)) value = false;
    else return {};

    return ast.make_literal_expr(
        ast::lang_bool,
        (int64_t)value
    );
}

}