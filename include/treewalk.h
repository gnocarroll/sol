#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "ast/ast_object.h"
#include "ast/instance.h"
#include "ast/type.h"
#include "lang_err.h"
#include "macros.h"
#include "mixins.h"

namespace treewalk {

class ExecutionErr : public LangErr {
public:
    const ast::ASTObject& source;

    ExecutionErr(const ast::ASTObject& source) :
        source(source) {}
    ExecutionErr(const ast::ASTObject& source, std::string&& err_msg) :
        LangErr(std::move(err_msg)), source(source) {}
};



class LiveValue {
    const ast::LangType& lang_type;

public:
    LiveValue(const ast::LangType& lang_type) : lang_type(lang_type) {}
    virtual ~LiveValue() {}

    const ast::LangType& get_lang_type() const {
        return lang_type;
    }
};

DEF_DERIVED_TYPES(LiveValue)

class LiveBooleanValue final : public LiveValue {
public:
    bool value = false;

    LiveBooleanValue(bool value) : LiveValue(ast::lang_bool), value(value) {}

    bool& operator()() {
        return value;
    }
};

class LiveIntegerValue final : public LiveValue {
public:
    int64_t value = 0;

    LiveIntegerValue(int64_t value) : LiveValue(ast::lang_integer), value(value) {}

    int64_t& operator()() {
        return value;
    }
};

class LiveErrValue final : public LiveValue {
public:

    LiveErrValue() : LiveValue(ast::lang_err_type) {}

    static LiveValuePtr create() {
        return std::make_unique<LiveErrValue>();
    }
};

class LiveInstance {
    LiveValuePtr value = LiveErrValue::create();
public:
    const ast::Instance& instance;

    LiveInstance(const ast::Instance& instance) :
        instance(instance) {}
    LiveInstance(const ast::Instance& instance, LiveValuePtr&& value) :
        instance(instance), value(std::move(value)) {}

    const LiveValuePtr& get_value() const {
        return value;
    }
    void set_value(LiveValuePtr&& new_value) {
        value = std::move(new_value);
    }
};

DEF_DERIVED_TYPES(LiveInstance)

class ExecutionContext : public ErrorRegistry<ExecutionErr> {
    std::unordered_map<std::string,LiveInstance> live_instances;
public:

    void add_live_instance(const ast::Instance& instance) {
        live_instances.emplace(std::make_pair(
            std::string(instance.name),
            LiveInstance(instance)
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