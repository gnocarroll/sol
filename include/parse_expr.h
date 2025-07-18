#pragma once

#include "char_stream.h"
#include "ast/expr.h"
#include "ast/scope.h"

ast::OptionalExprPtr parse_expr(CharStream& cstream, ast::Scope& scope);