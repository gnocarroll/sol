#pragma once

typedef enum operator_type {
    OP_TYPE_NONE,
    OP_TYPE_UNARY,
    OP_TYPE_BINARY,
} operator_type;

typedef enum operator_enum {

#define OP(name, tok) name ,

#include "def_operators.txt"

#undef OP

} operator_enum;
