#pragma once

#include "ast/statement.h"
#include "treewalk/treewalk.h"

namespace treewalk {
	void execute_statement(ExecutionContext& ctx, ast::Statement& statement);
}