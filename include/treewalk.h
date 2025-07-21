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

    bool is_err_value() const {
        return false;
    }

    virtual std::unique_ptr<LiveValue> clone_ptr() const = 0;

    virtual LiveValuePtr unary(Operator op) const = 0;
    virtual LiveValuePtr binary(Operator op, const LiveValuePtr& other) const = 0;

    virtual std::string to_string() const = 0;
};

DEF_DERIVED_TYPES(LiveValue)

class LiveBooleanValue final : public LiveValue {
public:
    bool value = false;

    LiveBooleanValue(bool value) : LiveValue(ast::lang_bool), value(value) {}

    bool& operator()() {
        return value;
    }
    const bool& operator()() const {
        return value;
    }

    LiveValuePtr clone_ptr() const {
        return std::make_unique<LiveBooleanValue>(value);
    }

    LiveValuePtr unary(Operator op) const;
    LiveValuePtr binary(Operator op, const LiveValuePtr& other) const;

    std::string to_string() const {
        return value ? "true" : "false";
    }
};

class LiveIntegerValue final : public LiveValue {
public:
    int64_t value = 0;

    LiveIntegerValue(int64_t value) : LiveValue(ast::lang_integer), value(value) {}

    int64_t& operator()() {
        return value;
    }
    const int64_t& operator()() const {
        return value;
    }

    LiveValuePtr clone_ptr() const {
        return std::make_unique<LiveIntegerValue>(value);
    }

    LiveValuePtr unary(Operator op) const;
    LiveValuePtr binary(Operator op, const LiveValuePtr& other) const;

    std::string to_string() const {
        return std::to_string(value);
    }
};

class LiveErrValue final : public LiveValue {
public:

    LiveErrValue() : LiveValue(ast::lang_err_type) {}

    static LiveValuePtr create() {
        return std::make_unique<LiveErrValue>();
    }

    bool is_err_value() const {
        return true;
    }

    LiveValuePtr clone_ptr() const {
        return LiveErrValue::create();
    }

    LiveValuePtr unary(Operator op) const;
    LiveValuePtr binary(Operator op, const LiveValuePtr& other) const;

    std::string to_string() const {
        return "ERR";
    }
};

class LiveInstance : public HasErrFlag {
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