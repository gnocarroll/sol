#pragma once

#include <expected>
#include <functional>
#include <memory>
#include <optional>

#include "ast/type.h"
#include "macros.h"

namespace treewalk {

class LiveValue {
    const ast::LangType& lang_type;

public:
    LiveValue(LiveValue& other) = delete;
    LiveValue(const ast::LangType& lang_type) : lang_type(lang_type) {}
    virtual ~LiveValue() {}

    const ast::LangType& get_lang_type() const {
        return lang_type;
    }

    bool is_err_value() const {
        return false;
    }

    virtual std::unique_ptr<LiveValue> clone_ptr() const = 0;

    virtual std::unique_ptr<LiveValue> unary(Operator op) const = 0;
    virtual std::unique_ptr<LiveValue> binary(
        Operator op,
        const std::unique_ptr<LiveValue>& other
    ) const = 0;

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

}