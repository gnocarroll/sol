#pragma once

#include "ast/statement.h"
#include "treewalk/treewalk.h"

namespace treewalk {
	void execute(ExecutionContext& ctx, ast::Statement& statement);
}