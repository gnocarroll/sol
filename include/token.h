#pragma once

#include <string_view>

class TokenType {
public:
    enum TokenTypeEnum {

#define TOK(name, text) name ,

#include "def_tokens.txt"

#undef TOK

    };

private:
    TokenTypeEnum value;

public:

    TokenType(TokenTypeEnum value) : value(value) {}

    bool operator==(const TokenType& other) {
        return value == other.value;
    }
    bool operator==(TokenTypeEnum value) {
        return this->value == value;
    }

    std::optional<std::string_view> get_text() const;
};