#pragma once

#include <optional>
#include <string>

#include "ast/ast_object.h"

namespace ast {

class Instance : public ASTObject {
public:
    const std::string name;
    std::optional<long> value;

    Instance(std::string&& name) : name(std::move(name)) {}
};

}