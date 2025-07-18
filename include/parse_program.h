#pragma once

#include "char_stream.h"
#include "ast/program.h"

ast::Program parse_program(CharStream &cstream);