#include "test_token.h"

#include <cctype>

static std::optional<size_t> match_integer(CharStream &cstream);
static std::optional<size_t> match_word(CharStream &cstream);

std::optional<size_t> match_token(CharStream &cstream, TokenType ttype) {
    switch (ttype()) {
    case TokenType::TOK_INTEGER:
        return match_integer(cstream);
    case TokenType::TOK_WORD:
        return match_word(cstream);
    default:
        break;
    }
    
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
        return matched;
    }

    return {};
}

static std::optional<size_t> match_integer(CharStream &cstream) {
    size_t n_chars = 0;

    while (true) {
        auto next_c = cstream.peekc();

        if (!next_c || !std::isdigit(*next_c)) break;

        cstream.getc(); // consume

        n_chars++;
    }

    if (n_chars == 0) return {};

    return n_chars;
}

static std::optional<size_t> match_word(CharStream &cstream) {
    size_t n_chars = 0;

    auto first_c = cstream.peekc();

    if (!first_c) return {};
    if (!std::isalpha(*first_c) && *first_c != '_') return {};

    cstream.getc();

    n_chars++;

    while (true) {
        auto next_c = cstream.peekc();

        if (!next_c || 
            (!std::isalnum(*next_c) && *next_c != '_')) break;

        cstream.getc(); // consume

        n_chars++;
    }

    return n_chars;
}
