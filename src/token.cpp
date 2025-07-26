#include "token.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static std::string_view token_text_arr[] = {

#define TOK(name, text) text ,

#include "def_tokens.txt"

#undef TOK

    ""
};

static std::string_view token_name_arr[]{

#define TOK(name, text) #name ,

#include "def_tokens.txt"

#undef TOK

    ""
};

std::optional<std::string_view> TokenType::get_text() const {
    if (value < 0 || value >= TokenType::TOK_COUNT) {
        return {};
    }

    std::string_view text = token_text_arr[(size_t) value];

    if (text == "") return {};

    return text;
}

std::optional<std::string_view> TokenType::get_name() const {
    if (value < 0 || value >= TokenType::TOK_COUNT) return {};

    return token_name_arr[(size_t)value];
}