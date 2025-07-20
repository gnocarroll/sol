#include "treewalk.h"

#include <cmath

namespace treewalk {
    LiveValuePtr LiveIntegerValue::unary(Operator op) const {
        auto ret = clone_ptr();

        LiveIntegerValue *ret_integer = dynamic_cast<LiveIntegerValue*>(
            ret.get()
        );

        switch (op()) {
        case Operator::OP_ADD:
            return ret;
            break;
        case Operator::OP_SUB:
            (*ret_integer)() = -((*this)());
            break;
        default:
            return LiveErrValue::create();
        }

        return ret;
    }

    LiveValuePtr LiveIntegerValue::binary(Operator op, const LiveValuePtr& other) const {
        LiveIntegerValue *other_integer = dynamic_cast<LiveIntegerValue*>(other.get());

        if (!other_integer) return LiveErrValue::create();

        switch (op()) {
        
        #define OP_CASE(name, op) case Operator:: OP_ ## name : \
            return std::make_unique<LiveIntegerValue>((*this)() op (*other_integer)());

        OP_CASE(ADD, +)
        OP_CASE(SUB, -)
        OP_CASE(MULT, *)
        OP_CASE(DIV, /)
        OP_CASE(MOD, %)

        case Operator::OP_POW:
            return std::make_unique<LiveIntegerValue>((int64_t) std::pow(
                (*this)(),
                (*other_integer)()
            ));
        default:
            break;

        }

        return LiveErrValue::create();
    }

    LiveValuePtr LiveBooleanValue::unary(Operator op) const {
        if (op() != Operator::OP_NOT) return LiveErrValue::create();

        auto ret = clone_ptr();

        LiveBooleanValue *ret_bool = dynamic_cast<LiveBooleanValue*>(ret.get());

        // apply NOT
        (*ret_bool)() = !(*this)();

        return ret;
    }

    LiveValuePtr LiveBooleanValue::binary(Operator op, const LiveValuePtr& other) const {
        LiveBooleanValue *other_bool = dynamic_cast<LiveBooleanValue*>(other.get());

        if (!other_bool) return LiveErrValue::create();
        
        switch (op()) {
        case Operator::OP_EQ:
            return std::make_unique<LiveBooleanValue>((*this)() == (*other_bool)());
        case Operator::OP_NEQ:
            return std::make_unique<LiveBooleanValue>((*this)() != (*other_bool)());
        default:
            break;
        }

        return LiveErrValue::create();
    }
    
    LiveValuePtr LiveErrValue::unary(Operator op) const {
        return LiveErrValue::create();
    }

    LiveValuePtr LiveErrValue::binary(Operator op, const LiveValuePtr& other) const {
        return LiveErrValue::create();
    }
}