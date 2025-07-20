#include "treewalk.h"

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

    }
    
    LiveValuePtr LiveErrValue::unary(Operator op) const {
        return LiveErrValue::create();
    }

    LiveValuePtr LiveErrValue::binary(Operator op, const LiveValuePtr& other) const {
        return LiveErrValue::create();
    }
}