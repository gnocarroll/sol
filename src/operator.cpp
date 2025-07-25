#include "operator.h"

static TokenType operator_token_arr[] = {

#define OP(name, ttype) TokenType:: ttype ,

#include "def_operators.txt"

#undef OP

    TokenType::TOK_NONE

};

std::optional<TokenType> Operator::get_token_type() const {
    if (value < 0 || value >= Operator::OP_COUNT) {
        return {};
    }

    TokenType ret = operator_token_arr[(size_t) value];

    if (ret == TokenType::TOK_NONE) return {};

    return ret;
}

std::optional<std::string_view> Operator::get_text() const {
    auto ttype = get_token_type();

    if (!ttype) return {};

    return ttype->get_text();
}