#pragma once

#include "char_stream.h"
#include "token.h"

bool match_token(CharStream &cstream, TokenType ttype) {
    auto checkpoint = cstream.scoped_checkpoint();

    auto ttext = ttype.get_text();

    if (!ttext) return false;

    size_t matched = 0;

    for (char c : *ttext) {
        auto stream_char = cstream.getc();

        if (!stream_char) break;

        if (c == *stream_char) matched++;
        else break;
    }

    if (matched == ttext->size()) {
        checkpoint.disable();
        return true;
    }

    return false;
}