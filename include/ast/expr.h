#pragma once

#include <cstdint>
#include <expected>
#include <functional>
#include <memory>
#include <optional>

#include "ast/ast_object.h"
#include "ast/instance.h"
#include "ast/type.h"
#include "ast/value.h"
#include "macros.h"
#include "operator.h"

namespace ast {

struct Expr : public ASTObject, public Value {
	Expr() {}
	Expr(Expr& other) = delete;
	
	enum ExprType {
		Err = 0,
		Binary,
		Unary,
		Literal,
		Instance,
	};

	ExprType _expr_type = ExprType::Err;

	int64_t _value = 0;

	ast::Instance* _instance = nullptr;

	Operator _op = Operator::OP_NONE;

	union {
		Expr* _lhs = nullptr;
		Expr* _sub_expr;
	};
	Expr* _rhs = nullptr;

	ExprType expr_type() const {
		return _expr_type;
	}

	std::optional<Operator> op() const {
		if (_expr_type != Unary && _expr_type != Binary) return {};
		if (_op == Operator::OP_NONE) return {};

		return _op;
	}
	std::optional<Expr*> lhs() const {
		if (!_lhs || _expr_type != Binary) return {};

		return _lhs;
	}
	std::optional<Expr*> rhs() const {
		if (!_rhs || _expr_type != Binary) return {};

		return _rhs;
	}
	std::optional<Expr*> sub_expr() const {
		if (!_sub_expr || _expr_type != Unary) return {};

		return _sub_expr;
	}
	std::optional<int64_t> value() const {
		if (_expr_type != Literal) return {};

		return _value;
	}
	std::optional<ast::Instance*> instance() const {
		if (!_instance || _expr_type != Instance) return {};

		return _instance;
	}
};

DEF_DERIVED_TYPES(Expr)

}