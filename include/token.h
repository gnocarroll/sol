#pragma once

#include <optional>
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

    bool operator==(const TokenType& other) const {
        return value == other.value;
    }
    bool operator==(TokenTypeEnum value) const {
        return this->value == value;
    }

    TokenTypeEnum operator()() const {
        return value;
    }

    std::optional<std::string_view> get_text() const;
};