#pragma once

#include "ast/program.h"
#include "ast/statement.h"
#include "treewalk/execution_context.h"

namespace treewalk {
	void execute_statement(ExecutionContext& ctx, ast::Statement& statement);
	void execute_program(ExecutionContext& ctx, ast::Program& program);
}