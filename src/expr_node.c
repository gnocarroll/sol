#include "expr_node.h"

#include <stddef.h>
#include <stdlib.h>

ExprNode get_expr_node() {
    ExprNode ret;

    ret.node_type = EXPR_NODE_ERR;

    ret.op_type = OP_TYPE_NONE;
    ret.op = OP_NONE;

    ret.lhs = NULL;
    ret.rhs = NULL;

    ret.value = 0;

    return ret;
}

ExprNode *alloc_expr_node() {
    ExprNode *ret = calloc(1, sizeof(ExprNode));

    if (!ret) return ret;

    *ret = get_expr_node();

    return ret;
}

void free_expr_tree(ExprNode *node) {
    if (!node) return;

    free_expr_tree(node->lhs);
    free_expr_tree(node->rhs);

    free(node);
}