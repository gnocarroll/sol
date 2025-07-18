#pragma once

#include <optional>
#include <string>

class Instance {
public:
    const std::string name;

    bool err = false;
    std::optional<long> value;

    Instance(std::string&& name) : name(std::move(name)) {}
};