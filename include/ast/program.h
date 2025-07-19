#pragma once

#include "treewalk.h"
#include "ast/ast_object.h"
#include "ast/scope.h"
#include "ast/statement.h"

namespace ast {

class Program : public ASTObject {
    Scope global_scope;
    CompoundStatement statements;

public:
    treewalk::ExecutionContext ctx;

    Program(Scope&& global_scope, CompoundStatement&& statements) :
        global_scope(global_scope), statements(std::move(statements)) {

        if (global_scope.has_err() || statements.has_err()) {
            set_err();
        }
    }

    void execute() {
        return statements.execute(ctx);
    }
};

}