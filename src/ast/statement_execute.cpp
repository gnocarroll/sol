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

    }
}

void CreateStatement::execute(treewalk::ExecutionContext& ctx) {
    if (instance.has_err() || !expr) return;

    auto val = (*expr)->eval(ctx);

    if (!val) {
        instance.set_err();
        return;
    }

    instance.value = (*val);
}

void ModifyStatement::execute(treewalk::ExecutionContext& ctx) {
    if (instance.has_err()) return;

    auto val = expr->eval(ctx);

    if (!val) {
        instance.set_err();
        return;
    }

    instance.value = *val;
}

void PrintStatement::execute(treewalk::ExecutionContext& ctx) {
    if (!expr) {
        std::cout << '\n';
        return;
    }

    auto val = (*expr)->eval(ctx);

    if (!val) {
        // in general do not register new errors if one has already occurred
        return;
    }

    std::cout << *val << '\n';

    return;
}

void ErrStatement::execute(treewalk::ExecutionContext& ctx) {
    return;
}

}