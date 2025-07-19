#include "ast/expr.h"

namespace ast {

std::optional<long> BinaryExpr::eval(treewalk::ExecutionContext& ctx) {
    auto lhs_val = lhs->eval(ctx);
    auto rhs_val = rhs->eval(ctx);

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
            ctx.register_error(
                *this,
                "attempted to execute binary expression containing unsupported/invalid operation"
            );
            break;
    }

    return {};
}

std::optional<long> UnaryExpr::eval(treewalk::ExecutionContext& ctx) {
    auto sub_expr_val = sub_expr->eval(ctx);

    if (!sub_expr_val) return {};

    switch (op()) {
        case Operator::OP_ADD:
            return *sub_expr_val;
        case Operator::OP_SUB:
            return -*sub_expr_val;
        default:
            ctx.register_error(
                *this,
                "attempted to execute unary expression with unsupported/invalid operator"
            );
            break;
    }

    return {};
}

std::optional<long> InstanceExpr::eval(treewalk::ExecutionContext& ctx) {
    if (instance.has_err()) return {};

    return instance.value;
}

}