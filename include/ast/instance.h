#pragma once

#include <memory>
#include <optional>
#include <expected>
#include <string>

#include "ast/ast_object.h"
#include "ast/value.h"
#include "macros.h"

namespace ast {

struct Instance : public ASTObject, public Value {
    std::string _name;

    Instance(Instance& other) = delete;

    const std::string& name() const {
        return _name;
    }
};

DEF_DERIVED_TYPES(Instance)

}