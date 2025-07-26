#pragma once

#include <memory>

#include "macros.h"
#include "treewalk/live_instance.h"

namespace treewalk {

class LiveScope {
	std::unordered_map<std::string, LiveInstance*> live_instance_map;

    std::optional<LiveScope*> _parent;
    std::vector<LiveScope*> _children;

public:
    std::optional<LiveScope*> parent() {
        return _parent;
    }

    std::vector<LiveScope*>& children() {
        return _children;
    }

    void assign_value(ast::Instance& instance, LiveValuePtr&& value) {
        auto name = std::string(instance.name());
        auto live_instance = live_instances.back().get();

        live_instance_map.emplace(std::move(name), live_instance);
    }

    std::optional<LiveInstance*> get_live_instance(const std::string& name) {
        if (!live_instance_exists(name)) return {};

        return *live_instance_map.at(name);
    }
    std::optional<LiveInstance*> get_live_instance_no_recurse(const std::string& name) {

    }
};

DEF_DERIVED_TYPES(LiveScope)

}