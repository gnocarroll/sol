#pragma once

#include "operator.h"

typedef enum expr_node_type {
    EXPR_NODE_ERR,
    EXPR_NODE_LITERAL,
    EXPR_NODE_OTHER,
} expr_node_type;

typedef struct expr_node {
    expr_node_type node_type;
    operator_type op_type;
    operator_enum op;

    union {
        struct expr_node *lhs;
        struct expr_node *child;
    };

    struct expr_node *rhs;

    int value;
} expr_node;

expr_node get_expr_node();

expr_node *alloc_expr_node();
void free_expr_tree(expr_node *);