#pragma once

typedef enum OperatorType {
    OP_TYPE_NONE,
    OP_TYPE_UNARY,
    OP_TYPE_BINARY,
} OperatorType;

typedef enum Operator {

#define OP(name, tok) name ,

#include "def_operators.txt"

#undef OP

} Operator;
