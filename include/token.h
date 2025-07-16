#pragma once

typedef enum token_type {

#define TOK(name, text) name ,

#include "def_tokens.txt"

#undef TOK

} token_type;

const char* get_token_text(token_type ttype);