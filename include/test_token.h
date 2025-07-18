#pragma once

#include <optional>

#include "char_stream.h"
#include "token.h"

std::optional<size_t> match_token(CharStream &cstream, TokenType ttype);