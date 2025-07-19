#pragma once

#include "execute.h"
#include "ast/ast_object.h"
#include "ast/scope.h"
#include "ast/statement.h"

namespace ast {

class Program : public ASTObject {
    Scope global_scope;
    CompoundStatement statements;

public:
    Program(Scope&& global_scope, CompoundStatement&& statements) :
        global_scope(global_scope), statements(std::move(statements)) {

        if (global_scope.has_err() || statements.has_err()) {
            set_err();
        }
    }

    RetCode execute() {
        return statements.execute();
    }
};

}