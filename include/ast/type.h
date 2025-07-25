#pragma once

#include <string>

#include "operator.h"

namespace ast {

class LangType;

using BinaryOpRetType = LangType& (*)(Operator op);
using UnaryOpRetType = LangType& (*)(Operator op);

class LangType {
    std::string name;

    BinaryOpRetType _get_binary_op_ret_type;
    UnaryOpRetType _get_unary_op_ret_type;

public:
    LangType(LangType& other) = delete;
    LangType(
        BinaryOpRetType _get_binary_op_ret_type,
        UnaryOpRetType _get_unary_op_ret_type
    ) :
        _get_binary_op_ret_type(_get_binary_op_ret_type),
        _get_unary_op_ret_type(_get_unary_op_ret_type) {}

    std::string_view get_name() const {
        return name;
    }

    LangType& get_binary_op_ret_type(Operator op) const {
        return _get_binary_op_ret_type(op);
    }

    LangType& get_unary_op_ret_type(Operator op) const {
        return _get_unary_op_ret_type(op);
    }
};

extern LangType lang_err_type;
extern LangType lang_bool;
extern LangType lang_integer;

}