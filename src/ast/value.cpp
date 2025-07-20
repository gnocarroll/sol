#include "ast/value.h"

namespace ast {
    Operator IntegerValue::supported_ops[] = {};
    size_t IntegerValue::supported_op_count = sizeof(Value::supported_ops) / sizeof(Operator);

    Operator BooleanValue::supported_ops[] = {};
    size_t BooleanValue::supported_op_count = sizeof(Value::supported_ops) / sizeof(Operator);

    
}