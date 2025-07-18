#pragma once

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "instance.h"

class Scope {
    std::unordered_map<std::string,Instance> instances;

public:
    Scope() {}

    void push(Instance &&instance) {
        std::string key = instance.name;

        instances.emplace(std::make_pair(
            std::move(key),
            std::move(instance))
        );
    }

    std::optional<std::reference_wrapper<Instance> > get(const std::string& name) {
        if (instances.count(name) == 0) return {};

        return instances.at(name);
    }
};