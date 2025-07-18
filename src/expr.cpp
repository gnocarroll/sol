#include "expr.h"

#include <cmath>

void BinaryExpr::print(std::ostream &ostream) {
    ostream << '(';
    auto ttype = op.get_token_type();

    if (!ttype) ostream << "ERR";
    else {
        auto ttext = ttype->get_text();

        if (!ttext) ostream << "ERR";
        else ostream << *ttext;
    }

    ostream << ' ';

    lhs->print(ostream);

    ostream << ' ';

    rhs->print(ostream);

    ostream << ')';
}

std::optional<long> BinaryExpr::eval() {
    auto lhs_val = lhs->eval();
    auto rhs_val = rhs->eval();

    if (!lhs_val || !rhs_val) return {};

    switch (op()) {
        case Operator::OP_ADD:
            return *lhs_val + *rhs_val;
        case Operator::OP_SUB:
            return *lhs_val - *rhs_val;
        case Operator::OP_MULT:
            return *lhs_val * *rhs_val;
        case Operator::OP_DIV:
            return *lhs_val / *rhs_val;
        case Operator::OP_MOD:
            return *lhs_val % *rhs_val;
        case Operator::OP_POW:
            return std::pow(*lhs_val, *rhs_val);
        default:
            break;
    }

    return {};
}

void UnaryExpr::print(std::ostream &ostream) {
    ostream << '(';
    auto ttype = op.get_token_type();

    if (!ttype) ostream << "ERR";
    else {
        auto text = ttype->get_text();

        if (!text) ostream << "ERR";
        else ostream << *text;
    }

    ostream << ' ';

    sub_expr->print(ostream);

    ostream << ')';
}

std::optional<long> UnaryExpr::eval() {
    auto sub_expr_val = sub_expr->eval();

    if (!sub_expr_val) return {};

    switch (op()) {
        case Operator::OP_ADD:
            return *sub_expr_val;
        case Operator::OP_SUB:
            return -*sub_expr_val;
        default:
            break;
    }

    return {};
}