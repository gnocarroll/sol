#include "ast/statement.h"

namespace ast {

RetCode CompoundStatement::execute() {
    for (auto& statement : statements) {
        auto stmt_ret = statement->execute();

        if (stmt_ret.is_err()) return stmt_ret;
    }

    return RetCode::ok();
}

RetCode CreateStatement::execute() {
    if (instance.err) return RetCode("error in instance");
    if (!expr) return RetCode::ok();

    auto val = (*expr)->eval();

    if (!val) {
        instance.err = true;
        return RetCode("expression evaluation failed");
    }

    instance.value = (*val);

    return RetCode::ok();
}

RetCode ModifyStatement::execute() {
    if (instance.err) return RetCode("error in instance");

    auto val = expr->eval();

    if (!val) {
        instance.err = true;
        return RetCode("expr evaluation failed");
    }

    instance.value = *val;

    return RetCode::ok();
}

RetCode PrintStatement::execute() {
    if (!expr) {
        std::cout << '\n';
        return RetCode::ok();
    }

    auto val = (*expr)->eval();

    if (!val) {
        return RetCode("expression evaluation failed");
    }

    std::cout << *val << '\n';

    return RetCode::ok();
}

RetCode ErrStatement::execute() {
    return RetCode("cannot execute malformed error statement");
}

}