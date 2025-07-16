#include "expr_node.h"

#include <stddef.h>

expr_node get_expr_node() {
    expr_node ret;

    ret.op_type = OP_TYPE_NONE;
    ret.op = OP_NONE;

    ret.lhs = NULL;
    ret.rhs = NULL;

    return ret;
}