#include "parse.h"

#include "test_token.h"

namespace parse {

static ast::OptionalStatementRef _parse_statement(ast::AST& ast, ast::Scope& scope);

ast::OptionalStatementRef parse_statement(ast::AST& ast, ast::Scope& scope) {
	auto ret = _parse_statement(ast, scope);

	if (!ret) return {};

	if (!parse_one_plus_newlines(ast)) {
		ast.register_error("expected at least one newline immediately following statement");
	}

	return ret;
}

ast::Statement& parse_compound_statement(ast::AST& ast, ast::Scope& scope) {
	std::vector<ast::Statement*> statements;
	
	parse_zero_plus_newlines(ast);

	while (true) {
		auto maybe_statement = parse_statement(ast, scope);

		if (!maybe_statement || ast.n_errs() > 0) break;

		statements.push_back(&maybe_statement->get());
	}

	parse_zero_plus_newlines(ast);

	return ast.make_compound_statement(std::move(statements));
}

static ast::OptionalStatementRef parse_print_statement(ast::AST& ast, ast::Scope& scope) {
	if (!match_token(ast.cstream, TokenType::TOK_PRINT)) return {};

	auto expr = parse_expr(ast, scope);

	// no expr, will just print newline
	if (!expr) return ast.make_print_statement();

	// create w/ expr
	return ast.make_print_statement(&expr->get());
}

static ast::OptionalStatementRef parse_block_statement(ast::AST& ast, ast::Scope& scope) {
	if (!match_token(ast.cstream, TokenType::TOK_BLOCK)) return {};



	expect_tok(ast, TokenType::TOK_END);
	expect_tok(ast, TokenType::TOK_BLOCK);
}

static ast::OptionalStatementRef parse_end_create_statement_w_expr(
	ast::AST& ast,
	ast::Scope& scope,
	const std::string& name
);

static ast::OptionalStatementRef parse_end_modify_statement(
	ast::AST& ast,
	ast::Scope& scope,
	const std::string& name
);

static ast::OptionalStatementRef _parse_statement(ast::AST& ast, ast::Scope& scope) {
	if (auto print_stmt = parse_print_statement(ast, scope)) return print_stmt;
	if (auto block_stmt = parse_block_statement(ast, scope)) return block_stmt;

	auto n_chars = match_token(ast.cstream, TokenType::TOK_WORD);

	if (!n_chars) return {};

	std::string name = ast.cstream.last_n_as_str(*n_chars)->get_str();

	if (auto create_stmt = parse_end_create_statement_w_expr(ast, scope, name)) return create_stmt;
	if (auto modify_stmt = parse_end_modify_statement(ast, scope, name)) return modify_stmt;

	ast.register_error("WORD should be followed by := or = to form statement");

	return ast.make_err_statement();
}

static ast::OptionalStatementRef parse_end_create_statement_w_expr(ast::AST& ast, ast::Scope& scope, const std::string& name) {
	if (!match_token(ast.cstream, TokenType::TOK_COLON_EQUAL)) return {};

	if (scope.get(name)) {
		ast.register_error(std::string("variable already exists: ") + name);

		return ast.make_err_statement();
	}

	auto expr = parse_expr(ast, scope);

	if (!expr) {
		ast.register_error(std::string("expected expr to initialize variable"));

		expr = ast.make_err_expr();
	}

	auto& instance = ast.make_instance(
		std::string(name),
		expr->get().lang_type()
	);

	scope.push(instance);

	return ast.make_create_statement(
		instance,
		*expr
	);
}

static ast::OptionalStatementRef parse_end_modify_statement(ast::AST& ast, ast::Scope& scope, const std::string& name) {
	if (!match_token(ast.cstream, TokenType::TOK_EQUAL)) return {};

	auto instance = scope.get(name);

	if (!instance) {
		ast.register_error(std::string("variable name not recognized: ") + name);

		return ast.make_err_statement();
	}

	auto expr = parse_expr(ast, scope);

	if (!expr) {
		ast.register_error(std::string("expected expression for use in modifying variable ") + name);

		expr = ast.make_err_expr();
	}

	return ast.make_modify_statement(
		*instance,
		*expr
	);
}

}