#pragma once

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

    bool operator==(const Operator& other) {
        return value == other.value;
    }
    bool operator==(OperatorEnum value) {
        return this->value == value;
    }

    std::optional<TokenType> get_token_type() const;
};
