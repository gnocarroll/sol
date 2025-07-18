#pragma once

#include "char_stream.h"
#include "ast/program.h"

namespace parse {

ast::Program parse_program(CharStream &cstream);

}