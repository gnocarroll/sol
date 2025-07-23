#include "ast/ast.h"

namespace ast {
	Expr& AST::make_binary_expr(Expr& lhs, Operator op, Expr& rhs) {

	}
	Expr& AST::make_unary_expr(Operator op, Expr& sub_expr) {

	}
	Expr& AST::make_literal_expr(const LangType& lang_type, int64_t value) {

	}

	Expr(AST& ast, Expr& lhs, Operator op, Expr& rhs) :
		_expr_type(Binary), _lhs(&lhs), _op(op), _rhs(&rhs) {

		const auto& lhs_type = lhs.lang_type();
		const auto& rhs_type = rhs.lang_type();

		if (&lhs_type != &rhs_type) return;

		_lang_type = &lhs_type.get_binary_op_ret_type(op);
	}
	Expr(AST& ast, Operator op, Expr& sub_expr) :
		_expr_type(Unary), _op(op), _sub_expr(&sub_expr) {

		_lang_type = &sub_expr.get_lang_type().get_unary_op_ret_type(op);
	}
	Expr(AST& ast, const LangType& lang_type, int64_t value) :
		_lang_type(&lang_type), _value(value) {
	}
}