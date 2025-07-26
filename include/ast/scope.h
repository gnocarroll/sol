#pragma once

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ast/ast_object.h"
#include "ast/instance.h"
#include "macros.h"

namespace ast {

struct Scope : public ASTObject {
    std::optional<Scope*> _parent;
    std::vector<Scope*> _children;

    std::unordered_map<std::string,Instance*> instances;

    Scope() {}

    std::optional<Scope*> parent() {
        return _parent;
    }

    std::vector<Scope*>& children() {
        return _children;
    }

    void push(Instance &instance) {
        std::string key = instance.name();

        instances.emplace(std::make_pair(
            std::move(key),
            &instance
        ));
    }

    std::optional<std::reference_wrapper<Instance> > get(const std::string& name) {
        // if not found in current scope, recursively search in parent
        
        if (instances.count(name) == 0) {
            if (!_parent) return {};

            return (**_parent).get(name);
        }

        return *instances.at(name);
    }

    std::optional<std::reference_wrapper<Instance> > get_no_recurse(const std::string& name) {
        if (instances.count(name) == 0) return {};

        return *instances.at(name);
    }
};

DEF_DERIVED_TYPES(Scope)

}