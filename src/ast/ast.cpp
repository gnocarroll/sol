#include "ast/ast.h"

namespace ast {

Expr& AST::expr_factory() {
	exprs.emplace_back(std::make_unique<Expr>());

	return *exprs.back().get();
}

Expr& AST::make_binary_expr(Expr& lhs, Operator op, Expr& rhs) {
	Expr& ret = expr_factory();

	ret._expr_type = Expr::Binary;

	ret._lhs = &lhs;
	ret._op = op;
	ret._rhs = &rhs;

	const auto& lhs_type = lhs.lang_type();
	const auto& rhs_type = rhs.lang_type();

	if (&lhs_type != &rhs_type) {
		register_error("lhs and rhs of binary expr are of different types");
	
		return ret;
	}

	ret._lang_type = &lhs_type.get_binary_op_ret_type(op);

	if (ret._lang_type == &ast::lang_err_type) {
		std::string_view op_text;

		auto maybe_op_text = op.get_text();

		op_text = maybe_op_text ? *maybe_op_text : "ERR";
		
		register_error(
			std::string("binary expr not supported: ") +
			std::string(lhs_type.get_name()) + " " +
			std::string(op_text) + " " +
			std::string(rhs_type.get_name())
		);
	}
	
	return ret;
}

Expr& AST::make_unary_expr(Operator op, Expr& sub_expr) {
	Expr& ret = expr_factory();

	ret._expr_type = Expr::Unary;

	ret._op = op;
	ret._sub_expr = &sub_expr;

	const auto& sub_expr_type = sub_expr.get_lang_type();

	ret._lang_type = &sub_expr_type.get_unary_op_ret_type(op);

	if (ret._lang_type == &ast::lang_err_type) {
		std::string_view op_text;

		auto maybe_op_text = op.get_text();

		op_text = maybe_op_text ? *maybe_op_text : "ERR";

		register_error(
			std::string("unary expr not supported: ") +
			std::string(op_text) + " " +
			std::string(sub_expr_type.get_name())
		);
	}

	return ret;
}

Expr& AST::make_literal_expr(const LangType& lang_type, int64_t value) {
	Expr& ret = expr_factory();
	
	ret._expr_type = Expr::Literal;
	
	ret._lang_type = &lang_type;
	ret._value = value;

	return ret;
}

}
