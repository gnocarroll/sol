#pragma once

#include "ast/type.h"

namespace ast {

class Value {
    const LangType* lang_type = &lang_err_type;

public:
    Value() {}
    Value(const LangType& lang_type) : lang_type(&lang_type) {}

    const LangType& get_lang_type() const {
        return *lang_type;
    }
    void set_lang_type(const LangType& _lang_type) {
        lang_type = &_lang_type;
    }
};

}