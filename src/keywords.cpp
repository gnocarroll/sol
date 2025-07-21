#include "keywords.h"

#include "token.h"

std::initializer_list<std::string_view> keywords = {

#define KW(name, tok) *TokenType(TokenType:: tok ).get_text(),

#include "def_keywords.txt"

    "if"

};