#pragma once

#include <optional>
#include <string>

#include "ast/ast_object.h"
#include "ast/value.h"
#include "macros.h"

namespace ast {

class Instance : public ASTObject, public Value {
public:
    const std::string name;

    Instance(std::string&& name) : name(std::move(name)) {}
    Instance(std::string&& name, const LangType& lang_type) :
        Value(lang_type), name(std::move(name)) {}
};

DEF_DERIVED_TYPES(Instance)

}