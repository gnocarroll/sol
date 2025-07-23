#include "treewalk/eval.h"

namespace treewalk {

treewalk::LiveValuePtr BinaryExpr::eval(treewalk::ExecutionContext& ctx) const {
    auto lhs_val = lhs->eval(ctx);
    auto rhs_val = rhs->eval(ctx);

    if (lhs_val->is_err_value() || rhs_val->is_err_value()) return lhs_val;

    auto ret = lhs_val->binary(op, rhs_val);

    if (ret->is_err_value()) {
        ctx.register_error(
            *this,
            "attempted unsupported binary operation"
        );
    }

    return ret;
}

treewalk::LiveValuePtr UnaryExpr::eval(treewalk::ExecutionContext& ctx) const {
    auto sub_expr_val = sub_expr->eval(ctx);

    if (sub_expr_val->is_err_value()) return sub_expr_val;

    auto ret = sub_expr_val->unary(op);

    if (ret->is_err_value()) {
        ctx.register_error(
            *this,
            "attempted unsupported unary operation"
        );
    }

    return ret;
}

treewalk::LiveValuePtr InstanceExpr::eval(treewalk::ExecutionContext& ctx) const {
    if (instance.has_err() ||
        !ctx.live_instance_exists(instance.name)) return treewalk::LiveErrValue::create();

    return (*ctx.get_live_instance(instance.name)).get().get_value()->clone_ptr();
}

}