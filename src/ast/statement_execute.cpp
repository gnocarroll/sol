#include "ast/statement.h"

namespace ast {

void CompoundStatement::execute(treewalk::ExecutionContext& ctx) {
    for (auto& statement : statements) {
        statement->execute(ctx);

        if (ctx.n_errs() > 0) return;
    }
}

void IfStatement::execute(treewalk::ExecutionContext& ctx) {    
    if (if_thens.size() == 0)

    for (auto& if_then : if_thens) {
        (void) if_then;
    }
}

void CreateStatement::execute(treewalk::ExecutionContext& ctx) {
    if (instance.has_err()) return;

    if (!expr) {
        ctx.add_live_instance(instance);
        return;
    }

    auto val = (*expr)->eval(ctx);

    ctx.add_live_instance(
        instance,
        std::move(val)
    );
}

void ModifyStatement::execute(treewalk::ExecutionContext& ctx) {
    if (instance.has_err()) return;

    auto optional_live_instance = ctx.get_live_instance(instance.name);

    if (!optional_live_instance) {
        ctx.register_error(
            *this,
            "attempted to run modify statement with unrecognized instance"
        );
        return;
    }

    treewalk::LiveInstance& live_instance = *optional_live_instance;

    auto val = expr->eval(ctx);

    live_instance.set_value(std::move(val));
}

void PrintStatement::execute(treewalk::ExecutionContext& ctx) {
    if (!expr) {
        std::cout << '\n';
        return;
    }

    auto val = (*expr)->eval(ctx);

    std::cout << val->to_string() << '\n';

    return;
}

void ErrStatement::execute(treewalk::ExecutionContext& ctx) {
    ctx.register_error(
        *this,
        "attempted to execute statement which was not correctly compiled"
    );
    return;
}

}