#include "parse_expr.h"

#include <initializer_list>

#include "operator.h"
#include "test_token.h"
#include "token.h"

#define N_PRECEDENCE_LEVELS (3)

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

static OptionalExprPtr parse_binary_expr(CharStream&);
static OptionalExprPtr parse_unary_expr(CharStream&);
static OptionalExprPtr parse_primary_expr(CharStream&);
static OptionalExprPtr parse_literal_expr(CharStream&);

OptionalExprPtr parse_expr(CharStream& cstream) {
    return parse_binary_expr(cstream);
}

static OptionalExprPtr _parse_binary_expr(CharStream& cstream, size_t precedence_idx) {
    auto parse_sub_expr = [&] {
        if (precedence_idx >= N_PRECEDENCE_LEVELS - 1) return parse_primary_expr(cstream);

        return _parse_binary_expr(cstream, precedence_idx + 1);
    };

    auto lhs = parse_sub_expr();

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

        auto rhs = parse_sub_expr();

        // TODO: print something on error, better error handling, idk
        if (!rhs) return {};

        lhs = std::make_unique<Expr>(BinaryExpr(
            std::move(*lhs),
            *found_op,
            std::move(*rhs)
        ));
    }

    return lhs;
}

static OptionalExprPtr parse_binary_expr(CharStream& cstream) {
    return _parse_binary_expr(cstream, 0);
}

static OptionalExprPtr parse_unary_expr(CharStream&) {
    
}

static OptionalExprPtr parse_primary_expr(CharStream&) {

}

static OptionalExprPtr parse_literal_expr(CharStream&) {

}