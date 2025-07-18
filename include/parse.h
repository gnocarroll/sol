#pragma once

#include "ast/expr.h"
#include "ast/program.h"
#include "ast/scope.h"
#include "char_stream.h"

namespace parse {

ast::OptionalExprPtr parse_expr(CharStream& cstream, ast::Scope& scope);

ast::Program parse_program(CharStream &cstream);

ast::OptionalStatementPtr parse_statement(CharStream& cstream, ast::Scope &scope);

bool parse_one_plus_newlines(CharStream&);
void parse_zero_plus_newlines(CharStream&);

}