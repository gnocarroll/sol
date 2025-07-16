#pragma once

#include "operator.h"

typedef struct expr_node {
    operator_type op_type;
    operator_enum op;

    union {
        struct expr_node *lhs;
        struct expr_node *child;
    };

    struct expr_node *rhs;
} expr_node;

expr_node get_expr_node();