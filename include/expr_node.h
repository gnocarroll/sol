#pragma once

#include "operator.h"

typedef enum ExprNodeType {
    EXPR_NODE_ERR,
    EXPR_NODE_LITERAL,
    EXPR_NODE_OTHER,
} ExprNodeType;

typedef struct ExprNode {
    ExprNodeType node_type;
    OperatorType op_type;
    Operator op;

    union {
        struct ExprNode *lhs;
        struct ExprNode *child;
    };

    struct ExprNode *rhs;

    int value;
} ExprNode;

ExprNode get_expr_node();

ExprNode *alloc_expr_node();
void free_expr_tree(ExprNode *);