#include "parse.h"

#include "test_token.h"

namespace parse {

std::optional<size_t> expect_tok(ast::AST& ast, TokenType ttype) {
	auto n_chars = match_token(ast.cstream, ttype);

	if (n_chars) return n_chars;

	auto tname = ttype.get_name();

	if (!tname) ast.register_error("tried to match on invalid token value (not one of enumerated values)");
	else {
		ast.register_error(
			std::string("expected but did not find token ") +
			std::string(*tname)
		);
	}

	return {};
}

}