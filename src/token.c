#include "token.h"

#include <stddef.h>

static const char *token_text_arr[] = {

#define TOK(name, text) text ,

#include "def_tokens.txt"

#undef TOK

    ""
};

const char *get_token_text(token_type ttype) {
    if (ttype < 0 || ttype >= TOK_COUNT) {
        return NULL;
    }

    return token_text_arr[(size_t) ttype];
}