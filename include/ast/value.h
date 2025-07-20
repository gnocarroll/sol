#pragma once

#include <array>
#include <span>

#include "operator.h"

namespace ast {

class Value {
public:
    virtual ~Value() {}

    virtual std::span<Operator> get_supported_ops() = 0;
};

class BooleanValue final : public Value {
    static Operator supported_ops[];
    static size_t supported_op_count;
public:
};

class IntegerValue final : public Value {
    static Operator supported_ops[];
    static size_t supported_op_count;
public:
};

}