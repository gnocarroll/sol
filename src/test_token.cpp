#include "test_token.h"

#include <cctype>

#include "keywords.h"

#define DECL_SPECIAL_MATCHER(name) \
    static std::optional<size_t> name (CharStream &cstream);

DECL_SPECIAL_MATCHER(match_eof)
DECL_SPECIAL_MATCHER(match_integer)
DECL_SPECIAL_MATCHER(match_word)

static void skip_ignored_ws(CharStream &cstream);

std::optional<size_t> match_token(CharStream &cstream, TokenType ttype) {    
    skip_ignored_ws(cstream);

    switch (ttype()) {
    case TokenType::TOK_EOF:
        return match_eof(cstream);
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

static std::optional<size_t> match_eof(CharStream &cstream) {
    // if there is a character then it is not EOF
    if (cstream.peekc()) return {};

    // EOF token is length 0 token
    return 0;
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
    auto checkpoint = cstream.scoped_checkpoint();

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

    std::string word = cstream.last_n_as_str(n_chars)->get_str();

    // ensure word is not a language keyword
    for (const auto& kw : keywords) {
        if (word == kw) return {};
    }

    checkpoint.disable();

    return n_chars;
}

static void skip_ignored_ws(CharStream &cstream) {    
    while (true) {
        auto maybe_next_char = cstream.peekc();

        if (!maybe_next_char) return;

        char c = *maybe_next_char;

        if (std::isspace(c) && c != '\n') {
            cstream.getc();
            continue;
        }

        return;
    }
}
