#include "expr_node.h"

#include <stddef.h>
#include <stdlib.h>

expr_node get_expr_node() {
    expr_node ret;

    ret.node_type = EXPR_NODE_ERR;

    ret.op_type = OP_TYPE_NONE;
    ret.op = OP_NONE;

    ret.lhs = NULL;
    ret.rhs = NULL;

    ret.value = 0;

    return ret;
}

expr_node *alloc_expr_node() {
    expr_node *ret = calloc(1, sizeof(expr_node));

    if (!ret) return ret;

    *ret = get_expr_node();

    return ret;
}

void free_expr_tree(expr_node *node) {
    if (!node) return;

    free_expr_tree(node->lhs);
    free_expr_tree(node->rhs);

    free(node);
}