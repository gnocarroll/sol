#include "ast/type.h"

namespace ast {

#define OP_CASE(name) case Operator:: OP_##name :

static LangType& bool_binary_ret_type(Operator op) {
    switch (op()) {
    OP_CASE(EQ) 
    OP_CASE(NEQ)
    OP_CASE(AND)
    OP_CASE(OR)
        return lang_bool;
    default:
        break;
    }

    return lang_err_type;
}

static LangType& bool_unary_ret_type(Operator op) {
    switch (op()) {
    OP_CASE(NOT)
        return lang_bool;
    default:
        break;
    }

    return lang_err_type;
}

static LangType& integer_binary_ret_type(Operator op) {
    switch (op()) {
    OP_CASE(ADD)
    OP_CASE(SUB)
    OP_CASE(MULT)
    OP_CASE(DIV)
    OP_CASE(MOD)
    OP_CASE(POW)
        return lang_integer;
    OP_CASE(LT)
    OP_CASE(LE)
    OP_CASE(GT)
    OP_CASE(GE)
    OP_CASE(EQ)
    OP_CASE(NEQ)
        return lang_bool;
    default:
        break;
    }

    return lang_err_type;
}

static LangType& integer_unary_ret_type(Operator op) {
    switch (op()) {
    OP_CASE(ADD)
    OP_CASE(SUB)
        return lang_integer;
    default:
        break;
    }

    return lang_err_type;
}

static LangType& err_binary_ret_type(Operator op) {
    return lang_err_type;
}

static LangType& err_unary_ret_type(Operator op) {
    return lang_err_type;
}

LangType lang_bool(bool_binary_ret_type, bool_unary_ret_type);
LangType lang_integer(integer_binary_ret_type, integer_unary_ret_type);
LangType lang_err_type(err_binary_ret_type, err_unary_ret_type);

}