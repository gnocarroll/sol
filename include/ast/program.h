#pragma once

#include "ast/ast_object.h"
#include "ast/scope.h"
#include "ast/statement.h"

namespace ast {

class Program : public ASTObject {
    Scope* _global_scope;
    Statement* _entry_point;
};

}