#include "ast/statement.h"
#include "treewalk/eval.h"
#include "treewalk/execute.h"

#include <iostream>

namespace treewalk {

#define DECL_EXECUTOR(TypePrefix, name) \
    static void execute_ ## name ## _statement(ExecutionContext& ctx, ast:: TypePrefix ## Statement & statement);

DECL_EXECUTOR(Block, block)
DECL_EXECUTOR(Compound, compound)
DECL_EXECUTOR(Create, create)
DECL_EXECUTOR(Modify, modify)
DECL_EXECUTOR(Print, print)

void execute_statement(ExecutionContext& ctx, ast::Statement& statement) {
#define STATEMENT_CASE(TypePrefix, lowercase) \
    if (auto* lowercase ## _statement = dynamic_cast<ast:: TypePrefix ## Statement *>(&statement)) { \
        execute_ ## lowercase ## _statement(ctx, * lowercase ## _statement ); \
        return; \
    }

    STATEMENT_CASE(Block, block)
    STATEMENT_CASE(Compound, compound)
    STATEMENT_CASE(Create, create)
    STATEMENT_CASE(Modify, modify)
    STATEMENT_CASE(Print, print)

    ctx.register_error(
        statement,
        "unable to recognize which statement type (e.g. compound statement) and execute"
    );
}

static void execute_block_statement(ExecutionContext& ctx, ast::BlockStatement& statement) {
    auto sub_stmt = statement.statement();

    if (!sub_stmt) {
        ctx.register_error(statement, "no sub-statement found in block statement");

        return;
    }

    execute_statement(ctx, **sub_stmt);
}

static void execute_compound_statement(ExecutionContext& ctx, ast::CompoundStatement& statement) {
    for (auto& statement : statement.statements()) {
        execute_statement(ctx, *statement);

        if (ctx.n_errs() > 0) return;
    }
}

static void execute_create_statement(ExecutionContext& ctx, ast::CreateStatement& statement) {
    auto maybe_instance = statement.instance();

    if (!maybe_instance) {
        ctx.register_error(statement, "found no instance associated with create statement");
        return;
    }

    auto instance = *maybe_instance;
    
    if (instance->has_err()) {
        ctx.register_error(statement, "error in contained instance");
        return;
    }

    auto maybe_expr = statement.expr();

    if (!maybe_expr) {
        ctx.register_error(statement, "expected expression in create statement");
        return;
    }

    auto val = eval(ctx, **maybe_expr);

    ctx.make_live_instance(
        *instance,
        std::move(val)
    );
}

static void execute_modify_statement(ExecutionContext& ctx, ast::ModifyStatement& statement) {
    auto instance = statement.instance();
    
    if (!instance || (**instance).has_err()) {
        ctx.register_error(statement, "modify statement does not contain valid instance");
        return;
    }

    auto optional_live_instance = ctx.get_live_instance(**instance);

    if (!optional_live_instance) {
        ctx.register_error(
            statement,
            "attempted to run modify statement with unrecognized instance"
        );
        return;
    }

    auto live_instance = *optional_live_instance;

    auto expr = statement.expr();

    if (!expr || (**expr).has_err()) {
        ctx.register_error(
            statement,
            "expr is invalid"
        );
        return;
    }

    auto val = eval(ctx, **expr);

    live_instance->set_value(std::move(val));
}

static void execute_print_statement(ExecutionContext& ctx, ast::PrintStatement& statement) {
    auto expr = statement.expr();
    
    if (!expr) {
        std::cout << '\n';
        return;
    }

    auto val = eval(ctx, **expr);

    std::cout << val->to_string() << '\n';

    return;
}

static void execute_err_statement(ExecutionContext& ctx, ast::ErrStatement& statement) {
    ctx.register_error(
        statement,
        "attempted to execute statement which was not correctly compiled"
    );
}

}