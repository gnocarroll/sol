#include "ast/ast.h"

namespace ast {

Instance& AST::instance_factory() {
	instances.emplace_back(std::make_unique<Instance>());

	return *instances.back().get();
}

Scope& AST::scope_factory() {
	scopes.emplace_back(std::make_unique<Scope>());

	return *scopes.back().get();
}

Instance& AST::make_instance(
	std::string&& name,
	const ast::LangType& lang_type
) {
	Instance& ret = instance_factory();

	ret._name = std::move(name);
	ret.set_lang_type(lang_type);

	return ret;
}

Expr& AST::expr_factory() {
	exprs.emplace_back(std::make_unique<Expr>());

	return *exprs.back().get();
}

Program& AST::program_factory() {
	programs.emplace_back(std::make_unique<Program>());

	return *programs.back().get();
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

	ret.set_lang_type(lhs_type.get_binary_op_ret_type(op));

	if (&ret.lang_type() == &ast::lang_err_type) {
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

	const auto& sub_expr_type = sub_expr.lang_type();

	ret.set_lang_type(sub_expr_type.get_unary_op_ret_type(op));

	if (&ret.lang_type() == &ast::lang_err_type) {
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
	
	ret.set_lang_type(lang_type);
	ret._value = value;

	return ret;
}

Expr& AST::make_instance_expr(Instance& instance) {
	Expr& ret = expr_factory();

	ret._expr_type = Expr::Instance;

	ret._instance = &instance;
	ret.set_lang_type(instance.lang_type());

	return ret;
}

Expr& AST::make_err_expr() {
	Expr& ret = expr_factory();

	ret.set_file_pos(cstream.get_file_pos());

	return ret;
}

Statement& AST::make_block_statement(Statement& statement) {
	auto& ret = statement_factory<BlockStatement>();

	ret._statement = &statement;

	return ret;
}

Statement& AST::make_compound_statement(std::vector<Statement*>&& statements) {
	auto& ret = statement_factory<CompoundStatement>();

	ret._statements = std::move(statements);

	return ret;
}

Statement& AST::make_create_statement(Instance& instance, Expr& expr) {
	auto& ret = statement_factory<CreateStatement>();

	ret._instance = &instance;
	ret._expr = &expr;

	return ret;
}

Statement& AST::make_modify_statement(Instance& instance, Expr& expr) {
	auto& ret = statement_factory<ModifyStatement>();

	ret._instance = &instance;
	ret._expr = &expr;

	return ret;
}

Statement& AST::make_print_statement(std::optional<Expr*> expr) {
	auto& ret = statement_factory<PrintStatement>();

	ret._expr = expr;

	return ret;
}

Statement& AST::make_err_statement() {
	auto& ret = statement_factory<ErrStatement>();

	ret.set_file_pos(cstream.get_file_pos());

	return ret;
}

Scope& AST::make_scope() {
	return scope_factory();
}

Program& AST::make_program(Scope& global_scope, Statement& statement) {
	auto& ret = program_factory();

	ret._global_scope = &global_scope;
	ret._entry_point = &statement;

	return ret;
}

}
