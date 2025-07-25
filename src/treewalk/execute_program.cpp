#include "treewalk/execute.h"

namespace treewalk {
	void execute_program(ExecutionContext& ctx, ast::Program& program) {
		auto entry_point = program.entry_point();
		auto global_scope = program.global_scope();
		
		if (!entry_point) ctx.register_error(program, "no entry point found for Program");
		if (!global_scope) ctx.register_error(program, "no global scope found in Program");

		if (ctx.n_errs() > 0) return;

		execute_statement(ctx, **entry_point);
	}
}