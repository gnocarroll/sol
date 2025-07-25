#pragma once

#include "ast/type.h"

namespace ast {

struct Value {
    const LangType* _lang_type = &lang_err_type;

    Value() {}
    Value(const LangType& lang_type) : _lang_type(&lang_type) {}

    const LangType& lang_type() const {
        return *_lang_type;
    }
    void set_lang_type(const LangType& lang_type) {
        _lang_type = &lang_type;
    }
};

}