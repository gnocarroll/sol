#pragma once

#include "ast/expr.h"
#include "treewalk/live_value.h"
#include "treewalk/treewalk.h"

namespace treewalk {
	LiveValuePtr eval(treewalk::ExecutionContext& ctx, ast::Expr& expr);
}