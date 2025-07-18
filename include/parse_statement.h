#pragma once

#include "char_stream.h"
#include "parse_expr.h"
#include "ast/scope.h"
#include "ast/statement.h"
#include "test_token.h"

ast::OptionalStatementPtr parse_statement(CharStream& cstream, ast::Scope &scope);