#include "treewalk/eval.h"

#include "ast/type.h"

namespace treewalk {

#define DECL_EVAL(expr_type) \
    static LiveValuePtr eval_ ## expr_type ## _expr (ExecutionContext& ctx, const ast::Expr& expr);

DECL_EVAL(binary)
DECL_EVAL(unary)
DECL_EVAL(literal)
DECL_EVAL(instance)

LiveValuePtr eval(ExecutionContext& ctx, const ast::Expr& expr) {
    switch (expr.expr_type()) {
    case ast::Expr::Binary:
        return eval_binary_expr(ctx, expr);
    case ast::Expr::Unary:
        return eval_unary_expr(ctx, expr);
    case ast::Expr::Literal:
        return eval_literal_expr(ctx, expr);
    case ast::Expr::Instance:
        return eval_instance_expr(ctx, expr);
    default:
        break;
    }

    ctx.register_error(expr, "expression type invalid");

    return LiveErrValue::create();
}

static LiveValuePtr eval_binary_expr(ExecutionContext& ctx, const ast::Expr& expr) {
    auto lhs = expr.lhs();
    auto rhs = expr.rhs();

    if (!lhs || !rhs) {
        ctx.register_error(expr, "binary expr missing subexpr");
        return LiveErrValue::create();
    }

    auto op = expr.op();

    if (!op) {
        ctx.register_error(expr, "binary expr missing op");
        return LiveErrValue::create();
    }
    
    auto lhs_val = eval(ctx, **lhs);
    auto rhs_val = eval(ctx, **rhs);

    if (lhs_val->is_err_value() || rhs_val->is_err_value()) return lhs_val;

    auto ret = lhs_val->binary(*op, rhs_val);

    if (ret->is_err_value()) {
        ctx.register_error(
            expr,
            "attempted unsupported binary operation"
        );
    }

    return ret;
}

static LiveValuePtr eval_unary_expr(ExecutionContext& ctx, const ast::Expr& expr) {
    auto sub_expr = expr.sub_expr();
    auto op = expr.op();

    if (!sub_expr) {
        ctx.register_error(expr, "unary expression missing sub_expr");
        return LiveErrValue::create();
    }
    if (!op) {
        ctx.register_error(expr, "unary expression missing op");
        return LiveErrValue::create();
    }
    
    auto sub_expr_val = eval(ctx, **sub_expr);

    if (sub_expr_val->is_err_value()) return sub_expr_val;

    auto ret = sub_expr_val->unary(*op);

    if (ret->is_err_value()) {
        ctx.register_error(
            expr,
            "attempted unsupported unary operation"
        );
    }

    return ret;
}

static LiveValuePtr eval_literal_expr(ExecutionContext& ctx, const ast::Expr& expr)  {
    auto value = expr.value();

    if (!value) {
        ctx.register_error(expr, "unable to retrieve value from literal expr");
        return LiveErrValue::create();
    }
    
    const auto& lang_type = expr.lang_type();

    if (&lang_type == &ast::lang_integer) {
        return std::make_unique<LiveIntegerValue>(*value);
    }
    else if (&lang_type == &ast::lang_bool) {
        return std::make_unique<LiveBooleanValue>(*value);
    }

    ctx.register_error(expr, "invalid lang type in expr");

    return LiveErrValue::create();
}

static LiveValuePtr eval_instance_expr(ExecutionContext& ctx, const ast::Expr& expr) {
    auto instance = expr.instance();

    if (!instance) {
        ctx.register_error(expr, "unable to retrieve instance from instance expr");
        return LiveErrValue::create();
    }

    if (!ctx.live_instance_exists((**instance).name())) {
        ctx.register_error(
            expr,
            std::string("unable to find already created instance with name ") + (**instance).name()
        );
        return LiveErrValue::create();
    }

    return (*ctx.get_live_instance((**instance).name())).get().get_value()->clone_ptr();
}

}