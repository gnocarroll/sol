#pragma once

#include "ast/ast_object.h"
#include "ast/scope.h"
#include "ast/statement.h"
#include "macros.h"

namespace ast {

struct Program : public ASTObject {
    Scope* _global_scope;
    Statement* _entry_point;

    std::optional<Scope*> global_scope() {
        if (!_global_scope) return {};

        return _global_scope;
    }

    std::optional<Statement*> entry_point() {
        if (!_entry_point) return {};

        return _entry_point;
    }
};

DEF_DERIVED_TYPES(Program)

}