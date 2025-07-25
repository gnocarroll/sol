#pragma once

#include <optional>

#include "token.h"

class Operator {
public:
    enum OperatorEnum {

#define OP(name, tok) name ,

#include "def_operators.txt"

#undef OP
    
    };

private:
    OperatorEnum value;

public:
    Operator(OperatorEnum value) : value(value) {}

    bool operator==(const Operator& other) const {
        return value == other.value;
    }
    bool operator==(OperatorEnum value) const {
        return this->value == value;
    }

    OperatorEnum operator()() const {
        return value;
    }

    std::optional<std::string_view> get_text() const;
    std::optional<TokenType> get_token_type() const;
};
