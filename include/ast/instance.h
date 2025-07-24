#pragma once

#include <optional>
#include <string>

#include "ast/ast_object.h"
#include "ast/value.h"
#include "macros.h"

namespace ast {

struct Instance : public ASTObject, public Value {
    std::string _name;

    Instance(Instance& other) = delete;
};

DEF_DERIVED_TYPES(Instance)

}