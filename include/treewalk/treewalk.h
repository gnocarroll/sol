#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "ast/ast_object.h"
#include "ast/instance.h"
#include "ast/type.h"
#include "lang_err.h"
#include "macros.h"
#include "mixins.h"
#include "treewalk/live_instance.h"

namespace treewalk {

class ExecutionErr : public LangErr {
public:
    const ast::ASTObject& source;

    ExecutionErr(const ast::ASTObject& source) :
        source(source) {}
    ExecutionErr(const ast::ASTObject& source, std::string&& err_msg) :
        LangErr(std::move(err_msg)), source(source) {}
};

class ExecutionContext : public ErrorRegistry<ExecutionErr> {
    std::unordered_map<std::string,LiveInstance> live_instances;
public:

    template <typename... Args>
    void add_live_instance(Args&&... args) {
        auto live_instance = LiveInstance(std::forward<Args>(args)...);
        auto name = std::string(live_instance.instance.name);

        live_instances.emplace(std::make_pair(
            std::move(name),
            std::move(live_instance)  
        ));
    }
    bool live_instance_exists(const std::string& name) const {
        return live_instances.count(name) != 0;
    }
    OptionalLiveInstanceRef get_live_instance(const std::string& name) {
        if (!live_instance_exists(name)) return {};

        return live_instances.at(name);
    }
};

}