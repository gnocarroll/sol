#pragma once

typedef enum TokenType {

#define TOK(name, text) name ,

#include "def_tokens.txt"

#undef TOK

} TokenType;

const char* get_token_text(TokenType ttype);