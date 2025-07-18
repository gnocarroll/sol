#pragma once

#include "char_stream.h"
#include "parse_expr.h"
#include "scope.h"
#include "statement.h"
#include "test_token.h"

OptionalStatementPtr parse_statement(CharStream& cstream, Scope &scope);