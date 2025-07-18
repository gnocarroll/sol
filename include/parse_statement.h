#pragma once

#include "char_stream.h"
#include "parse_expr.h"
#include "test_token.h"
#include "statement.h"

OptionalStatementPtr parse_statement(CharStream& cstream);