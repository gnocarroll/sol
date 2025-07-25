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
    LiveInstancePtrVec live_instances;
    std::unordered_map<std::string,LiveInstance*> live_instance_map;
public:

    void add_live_instance(ast::Instance& instance, LiveValuePtr&& value) {
        live_instances.emplace_back(std::make_unique<LiveInstance>(instance, std::move(value)));
        
        auto name = std::string(instance.name());
        auto live_instance = live_instances.back().get();

        live_instance_map.emplace(std::move(name), live_instance);
    }
    bool live_instance_exists(const std::string& name) const {
        return live_instance_map.count(name) != 0;
    }
    OptionalLiveInstanceRef get_live_instance(const std::string& name) {
        if (!live_instance_exists(name)) return {};

        return *live_instance_map.at(name);
    }
};

}