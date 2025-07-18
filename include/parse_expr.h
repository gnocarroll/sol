#pragma once

#include "char_stream.h"
#include "expr.h"
#include "scope.h"

OptionalExprPtr parse_expr(CharStream& cstream, Scope& scope);