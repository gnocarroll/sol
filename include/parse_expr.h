#pragma once

#include "char_stream.h"
#include "expr.h"

OptionalExprPtr parse_expr(CharStream& cstream);