#pragma once

#include "char_stream.h"

namespace parse {

bool parse_one_plus_newlines(CharStream&);
void parse_zero_plus_newlines(CharStream&);

}