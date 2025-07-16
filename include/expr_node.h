#pragma once

typedef struct expr_node {
    union {
        struct expr_node *lhs;
        struct expr_node *child;
    };

    struct expr_node *rhs;
} expr_node;