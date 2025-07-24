#pragma once

#include "ast/ast.h"
#include "ast/expr.h"
#include "ast/program.h"
#include "ast/scope.h"
#include "char_stream.h"

namespace parse {

ast::OptionalExprRef parse_expr(ast::AST& ast, ast::Scope& scope);

ast::Program& parse_program(ast::AST& ast);

ast::OptionalStatementRef parse_statement(ast::AST& ast, ast::Scope &scope);

ast::Statement& parse_compound_statement(ast::AST& ast, ast::Scope& scope);

bool parse_one_plus_newlines(ast::AST& ast);
void parse_zero_plus_newlines(ast::AST& ast);

}