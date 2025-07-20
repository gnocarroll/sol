#pragma once

#include <cstdint>
#include <string>

#include "ast/ast_object.h"
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

class ExecutionContext : public ErrorRegistry<ExecutionErr> {

};

class Value {
public:
    virtual ~Value() {}

    virtual const ast::LangType& get_lang_type() const = 0;
};

DEF_DERIVED_TYPES(Value)

class BooleanValue final : public Value {
    const ast::LangType& lang_type = ast::lang_bool;

public:
    bool value = false;

    BooleanValue(bool value) : value(value) {}

    bool& operator()() {
        return value;
    }

    const ast::LangType& get_lang_type() const {
        return lang_type;
    }
};

class IntegerValue final : public Value {
    const ast::LangType& lang_type = ast::lang_integer;

public:
    int64_t value = 0;

    IntegerValue(int64_t value) : value(value) {}

    int64_t& operator()() {
        return value;
    }

    const ast::LangType& get_lang_type() const {
        return lang_type;
    }
};

}