#pragma once

#include "ast/ast_builder.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/scope.h"
#include "char_stream.h"

namespace parse {

ast::OptionalExprPtr parse_expr(ast::AST& ast_builder, ast::Scope& scope);

ast::Program parse_program(ast::AST& ast_builder);

ast::OptionalStatementPtr parse_statement(ast::AST& ast_builder, ast::Scope &scope);

bool parse_one_plus_newlines(ast::AST& ast_builder);
void parse_zero_plus_newlines(ast::AST& ast_builder);

}