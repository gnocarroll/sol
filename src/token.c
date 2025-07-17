#include "token.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *token_text_arr[] = {

#define TOK(name, text) text ,

#include "def_tokens.txt"

#undef TOK

    ""
};

const char *get_token_text(TokenType ttype) {
    if (ttype < 0 || ttype >= TOK_COUNT) {
        return NULL;
    }

    return token_text_arr[(size_t) ttype];
}