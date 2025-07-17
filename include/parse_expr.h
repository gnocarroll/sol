#pragma once

#include <stddef.h>

#include "expr_node.h"

ExprNode *parse_expr(size_t n_chars, const char *s);