#include "ast/expr.h"

namespace ast {

treewalk::LiveValuePtr BinaryExpr::eval(treewalk::ExecutionContext& ctx) {
    auto lhs_val = lhs->eval(ctx);
    auto rhs_val = rhs->eval(ctx);

    if (lhs_val->is_err_value() || rhs_val->is_err_value()) return lhs_val;

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

    return treewalk::LiveErrValue::create();
}

treewalk::LiveValuePtr UnaryExpr::eval(treewalk::ExecutionContext& ctx) {
    auto sub_expr_val = sub_expr->eval(ctx);

    if (sub_expr_val->is_err_value()) return sub_expr_val;

    if (auto integer_value = dynamic_cast<treewalk::LiveIntegerValue*>(
        sub_expr_val.get()
    )) {
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
    }
    else if (auto boolean_value = dynamic_cast<treewalk::LiveBooleanValue*>(
        sub_expr_val.get()
    )) {

    }

    return treewalk::LiveErrValue::create();
}

treewalk::LiveValuePtr InstanceExpr::eval(treewalk::ExecutionContext& ctx) {
    if (instance.has_err() ||
        !ctx.live_instance_exists(instance.name)) return treewalk::LiveErrValue::create();

    return (*ctx.get_live_instance(instance.name)).get().get_value()->clone_ptr();
}

}